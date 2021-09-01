/* (c) Copyright 2020 Xilinx, Inc.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 * 		http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */

#ifndef __INCLUDE_H__
#define __INCLUDE_H__

// Downlink Beamforming parameters
#define NUM_PRBS     8

// In Out Parameters
#define SHFT              17
#define IN_DATA_BLKSZ     (NUM_PRBS*8*12)
#define IN_COEF_BLKSZ     (NUM_PRBS*8*8)
#define OUT_DATA_BLKSZ    (NUM_PRBS*8*12)
#define IN_DATA_WINSZ     (4*IN_DATA_BLKSZ)
#define IN_COEF_WINSZ     (4*IN_COEF_BLKSZ)
#define OUT_DATA_WINSZ    (4*OUT_DATA_BLKSZ)

#define READSCD (getc_scd())
#define READSS0 (getc_wss(0))


#endif /* __INCLUDE_H__ */
