/* umpl_interface.c
 * Pat Hickey, May 2011
 * Read a rotation matrix packet (type 10) 
 * from the debug serial port.
 */

#include <pulse_os.h>
#include "umpl_interface.h"

static uint8_t umpl_parse_input(uint8_t in, int8_t * out)
{   
    // Return flag indicates we wrote to *out.
    uint8_t returnflag = 0;
    // Internal state:
    static uint8_t input_valid = 0; // Current packet is valid (correct packet type)
    static uint8_t input_idx = 0;   // Index of in
    static uint8_t buf[9];          // Buffer for rotation matrix payload.
    
    if (input_valid){
        switch (input_idx)
        {
        case 0:
            if (in == '$') {     // Packet starts with '$'
                input_idx++;
            } else {
                input_valid = 0;
            }
        break;
        case 1:
            if (in == 10) {      // Second byte should be 10
                input_idx++;     // (we use other values here to
            } else {             // indicate other packet types)
                input_valid = 0;
            }
        break;
        case 2:  // bytes 2 through 10 are
        case 3:  // the rotation matrix payload
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            buf[input_idx-2] = in;
            input_idx++;
        break;
        case 11:
            // now we have the whole payload, and can write it to *out
            {
                uint8_t ii;
                for (ii = 0; ii < 9; ii++)
                    out[ii] = (int8_t) buf[ii]; // Copy buf to *out.
                returnflag = 1;                 // Tell caller we wrote to *out.
                dbg_putc("1"); // (for debugging)
            }
            input_idx++;
        break;
        case 12:
            input_idx++;
        break;
        case 13:          // packets are 14 bytes long.
            input_idx = 0;
        break;
        default:
            input_valid = 0;
        }
    } else { // input_valid = 0
        if (in == '$')  // '$' is always the packet start character.
        {
            input_valid = 1;
            input_idx = 1;
        }
    }
    return returnflag;
}


uint8_t umpl_input_handler(int8_t * out)
{
    uint8_t returnflag = 0;

    uint8_t got;
    uint8_t in;

    // read from the uart 1 byte at a time
    // dbg_get_uart is non-blocking, and backed
    // by the 16-byte hardware UART buffer.
    got = dbg_get_uart(&in);
    // The incoming packets are 14 bytes long,
    // and should arrive every 10ms.
    // We want to always read until the UART buffer
    // is empty.
    while (got > 0)
    {
        returnflag |= umpl_parse_input(in,out);
        got = dbg_get_uart(&in);
    }
    // returnflag indicates we wrote to *out
    return returnflag;
}


