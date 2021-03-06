/*
 * output_format.cpp
 *
 * This is shows the current QKD system snapshot
 *
 * Author: Manuel Warum, <manuel.warum@ait.ac.at>
 *
 * Copyright (C) 2012-2016 AIT Austrian Institute of Technology
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


#include "json_output.h"
#include "output_format.h"
#include "tabular_output.h"


// ------------------------------------------------------------
// code


/**
 * Creates a new instance depending on the provided options.
 * 
 * @param   cOptions   a data container with relevant formatting and output options.
 * @return  an already initialized output_format instance.
 */
std::shared_ptr<output_format> output_format::create(configuration_options const & cOptions) {
    
    std::shared_ptr<output_format> returnValue;
    
    if (cOptions.bOutputAsJSON) {
        returnValue = std::shared_ptr<output_format>(new json_output(cOptions));
    }
    else {
        returnValue = std::shared_ptr<output_format>(new tabular_output(cOptions));
    }
    
    return returnValue;
}
