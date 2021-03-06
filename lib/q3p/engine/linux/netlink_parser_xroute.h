/*
 * netlink_parser_xroute.h
 * 
 * parse NEW_ROUTE, DEL_ROUTE, GET_ROUTE answers from kernel
 *
 * Author: Oliver Maurhart, <oliver.maurhart@ait.ac.at>
 *
 * Copyright (C) 2016 AIT Austrian Institute of Technology
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


// this is only Linux code
#if defined(__linux__)
 
#ifndef __QKD_Q3P_LINUX_NETLINK_PARSER_XROUTE_H_
#define __QKD_Q3P_LINUX_NETLINK_PARSER_XROUTE_H_


// ------------------------------------------------------------
// incs

#include "netlink_parser.h"
#include "netlink_message.h"


// ------------------------------------------------------------
// decls


namespace qkd {
    
namespace q3p {    

   
/**
 * Wrapper for rtattr
 */
class netlink_parser_xroute : public netlink_parser {
    

public:
    
    
    /**
     * parse the message stored in cBuffer and add result to cMessage
     * 
     * @param   cMessage            message object to be filled
     * @param   cBuffer             memory returned from the kernel
     * @param   nSize               size of memory blob returned from the kernel
     * @return  true, if succefully parsed
     */
    virtual bool parse(netlink_message & cMessage, char * cBuffer, uint32_t nSize);
    
};
  

}

}


#endif

#endif
