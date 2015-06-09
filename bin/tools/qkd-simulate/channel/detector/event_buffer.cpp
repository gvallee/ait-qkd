/*
 * event_buffer.cpp
 * 
 * Implementation of an event buffer to be used inside detector at Alice/Bob sides
 *
 * Autor: Philipp Grabenweger
 * Autor: Oliver Maurhart, <oliver.maurhart@ait.ac.at>
 *
 * Copyright (C) 2013-2015 AIT Austrian Institute of Technology
 * AIT Austrian Institute of Technology GmbH
 * Donau-City-Strasse 1 | 1220 Vienna | Austria
 * http://www.ait.ac.at
 * 
 * This file is part of the AIT QKD Software Suite.
 *
 * The AIT QKD Software Suite is free software: you can redistribute 
 * it and/or modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation, either version 3 of 
 * the License, or (at your option) any later version.
 * 
 * The AIT QKD Software Suite is distributed in the hope that it will 
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty 
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with the AIT QKD Software Suite. 
 * If not, see <http://www.gnu.org/licenses/>.
 */


// ------------------------------------------------------------
// incs

#include <fstream>
#include <iomanip>

// ait
#include "event_buffer.h"
#include "../channel_event_manager.h"
#include "../photon_pair.h"


using namespace qkd::simulate;


// -----------------------------------------------------------------
// code


/**
 * handle a channel event. 
 * 
 * @param   cEvent      the channel event to be handled
 */
void event_buffer::handle(event const & cEvent) {
    
    switch (cEvent.eType) {
        
    case event::type::DETECTOR_PULSE: 
        
        // detector pulse event
        if (m_bWindowOpen) { 
            
            // store event to latch if in opened state
            switch (cEvent.cData.m_ePhotonState) {
                
            case photon_state::HORIZONTAL:
                m_det_latch[0] = true;
                break;
            
            case photon_state::VERTICAL:
                m_det_latch[1] = true;
                break;
            
            case photon_state::PLUS:
                m_det_latch[2] = true;
                break;
            
            case photon_state::MINUS:
                m_det_latch[3] = true;
                break;
            
            default:
                break;
            }
        }
        break;
        
    case event::type::INIT: 
        
        // simulation initialization
        for (uint64_t i = 0ULL; i < m_nBufferSize; i++) {
            m_cBuffer[i] = 0;
        }
        m_bNextHigh = true;
        m_nNextIndex = 0ULL;
        m_bWindowOpen = false;
    
        break;
    
    case event::type::SYNC_PULSE_BAD: 
        
        // sync pulse received while not all detectors were ready 
        m_det_latch[0] = false;
        m_det_latch[1] = false;
        m_det_latch[2] = false;
        m_det_latch[3] = false;
        // store zero record to event buffer
        write_event(); 
        break;
        
    case event::type::WINDOW_END: 
    
        // end of window generated by window generator
        if (m_bWindowOpen) write_event();
        m_bWindowOpen = false;
        break;
        
    case event::type::WINDOW_END_BAD: 
        
        // end of window due to a sync pulse received while not all detection elements were ready
        if (m_bWindowOpen) {
            write_event(); // write events stored so far in latch to event buffer
        }
        m_bWindowOpen = false;
        
        m_det_latch[0] = false;
        m_det_latch[1] = false;
        m_det_latch[2] = false;
        m_det_latch[3] = false;
        
        // write zero entry to event buffer for sync pulse coming while not all detection elements were ready
        write_event(); 
    
        break;
        
    case event::type::WINDOW_START: 
        
        // start of window generated by window generator
        if (m_bWindowOpen) {
            write_event();
        }
        
        m_det_latch[0] = false;
        m_det_latch[1] = false;
        m_det_latch[2] = false;
        m_det_latch[3] = false;
        m_bWindowOpen = true;
    
        break;
    
    default:
        break;
    }
}


/**
 * set event buffer size
 * 
 * @param   nBufferSize     the event buffer size in bytes
 */
void event_buffer::set_buffer_size(uint64_t nBufferSize) {
    
    // no change: exit
    if (nBufferSize == m_nBufferSize) return;

    // free mem and allocate new
    if (m_cBuffer) {
        delete[] m_cBuffer;
        m_cBuffer = nullptr;
    }
    if (nBufferSize > 0) m_cBuffer = new unsigned char[nBufferSize];
    
    m_nBufferSize = nBufferSize;
}


/**
 * write events currently in detector latch to event buffer (if not full already)
 */
void event_buffer::write_event() {
    
    // if already full: exit
    if (m_nNextIndex >= m_nBufferSize) return;
    
    unsigned char evmask;
    
    evmask = 0;
    if (m_det_latch[0]) evmask |= 1;
    if (m_det_latch[1]) evmask |= 2;
    if (m_det_latch[2]) evmask |= 4;
    if (m_det_latch[3]) evmask |= 8;
    
    if (m_bNextHigh) {
        m_cBuffer[m_nNextIndex] = (m_cBuffer[m_nNextIndex] & 0x0F) | (evmask << 4);
        m_bNextHigh = false;
    }
    else {
        m_cBuffer[m_nNextIndex] = (m_cBuffer[m_nNextIndex] & 0xF0) | evmask;
        m_bNextHigh = true;
        m_nNextIndex++;
    }
}


/**
 * write out all parameters of this event handler and its child event handlers
 * 
 * @param   cStream     the stream to write to
 */
void event_buffer::write_parameters(std::ofstream & cStream) {

    cStream << "NAME: " << get_name() << std::endl;
    cStream << "m_cBuffer: ";
    for (uint64_t i = 0; i < m_nBufferSize; i++) {
        cStream << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(m_cBuffer[i]) << std::dec << std::setfill(' ') << ' ';
    }
    cStream << std::endl;
    cStream << "m_nBufferSize: " << m_nBufferSize << std::endl;
    cStream << "m_det_latch: " << m_det_latch[0] << m_det_latch[1] << m_det_latch[2] << m_det_latch[3] << std::endl;
    cStream << "m_bNextHigh: " << m_bNextHigh << std::endl;
    cStream << "m_nNextIndex: " << m_nNextIndex << std::endl;
    cStream << "m_bWindowOpen: " << m_bWindowOpen << std::endl;
    cStream << std::endl; 
}