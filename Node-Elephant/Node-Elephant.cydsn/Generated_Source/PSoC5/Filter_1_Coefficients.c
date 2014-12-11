#include "Filter_1.h"
#include "Filter_1_PVT.h"


/*******************************************************************************
* ChannelA filter coefficients.
* Filter Type is FIR
*******************************************************************************/

/* Renamed array for backward compatibility.
*  Should not be used in new designs.
*/
#define ChannelAFirCoefficients Filter_1_ChannelAFirCoefficients

/* Number of FIR filter taps are: 16 */

const uint8 CYCODE Filter_1_ChannelAFirCoefficients[Filter_1_FIR_A_SIZE] = 
{
 0x4Eu, 0x4Du, 0x00u, 0x00u, /* Tap(0), 0.00235915184020996 */

 0x93u, 0x8Eu, 0x00u, 0x00u, /* Tap(1), 0.00435101985931396 */

 0xDDu, 0x08u, 0x01u, 0x00u, /* Tap(2), 0.00808298587799072 */

 0x9Au, 0xABu, 0x01u, 0x00u, /* Tap(3), 0.0130493640899658 */

 0x9Fu, 0x5Eu, 0x02u, 0x00u, /* Tap(4), 0.0185126066207886 */

 0x25u, 0x06u, 0x03u, 0x00u, /* Tap(5), 0.0236250162124634 */

 0x8Bu, 0x87u, 0x03u, 0x00u, /* Tap(6), 0.0275739431381226 */

 0xF6u, 0xCDu, 0x03u, 0x00u, /* Tap(7), 0.0297229290008545 */

 0xF6u, 0xCDu, 0x03u, 0x00u, /* Tap(8), 0.0297229290008545 */

 0x8Bu, 0x87u, 0x03u, 0x00u, /* Tap(9), 0.0275739431381226 */

 0x25u, 0x06u, 0x03u, 0x00u, /* Tap(10), 0.0236250162124634 */

 0x9Fu, 0x5Eu, 0x02u, 0x00u, /* Tap(11), 0.0185126066207886 */

 0x9Au, 0xABu, 0x01u, 0x00u, /* Tap(12), 0.0130493640899658 */

 0xDDu, 0x08u, 0x01u, 0x00u, /* Tap(13), 0.00808298587799072 */

 0x93u, 0x8Eu, 0x00u, 0x00u, /* Tap(14), 0.00435101985931396 */

 0x4Eu, 0x4Du, 0x00u, 0x00u, /* Tap(15), 0.00235915184020996 */
};

