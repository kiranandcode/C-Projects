#include "deflate.h"
#include <assert.h>
#include <stdio.h>
#define B bitstream_B


void deflate_insert(B stream, unsigned int length) {

	switch(length) {
		case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 10:
		/* code 257-264, 0 extra bits */
		{
	unsigned char first_byte = ((unsigned int) 254 + length) - 256;
	bitstream_insert(stream, first_byte, 8);
	bitstream_insert(stream, 1, 1);
		}
			break;
		case 11:case 12:
		/* code 265 1 extra bits */
		{
			unsigned char first_byte = ((unsigned int) 265) - 256;
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
			if(length == 11)
				bitstream_insert(stream, 0, 1);
			else   bitstream_insert(stream, 1, 1);
			printf("finished\n");
		}
			break;
		case 13:case 14:
		/* code 266 1 extra bits */
		{
			unsigned char first_byte = ((unsigned int) 266) - 256;
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
			if(length == 13)
				bitstream_insert(stream, 0, 1);
			else   bitstream_insert(stream, 1, 1);
		}
			break;
		case 15:case 16:
		/* code 267 1 extra bits */
		{
			unsigned char first_byte = ((unsigned int) 267) - 256;
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
			if(length == 15)
				bitstream_insert(stream, 0, 1);
			else   bitstream_insert(stream, 1, 1);
		}
			break;
		case 17:case 18:
		/* code 268 1 extra bits */
		{
			unsigned char first_byte = ((unsigned int) 268) - 256;
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
			if(length == 17)
				bitstream_insert(stream, 0, 1);
			else   bitstream_insert(stream, 1, 1);
		}
			break;
		case 19:case 20:case 21:case 22:
		/* code 269 2 extra bits */
		{
			unsigned char first_byte = ((unsigned int) 269) - 256;
			unsigned char extra_bits = (length - 19);
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
			bitstream_big_endian_insert(stream, extra_bits, 2);
		}	
			break;
		case 23:case 24:case 25:case 26:
		/* code 270 2 extra bits */
		{
			unsigned char first_byte = ((unsigned int) 270) - 256;
			unsigned char extra_bits = (length - 23);
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
			bitstream_big_endian_insert(stream, extra_bits, 2);
		}
			break;
		case 27:case 28:case 29:case 30:
		/* code 271 2 extra bits */
		{
			unsigned char first_byte = ((unsigned int) 271) - 256;
			unsigned char extra_bits = (length - 27);
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
			bitstream_big_endian_insert(stream, extra_bits, 2);
		}
			break;
		case 31:case 32:case 33:case 34:
		/* code 272 2 extra bits */
		{
			unsigned char first_byte = ((unsigned int) 272) - 256;
			unsigned char extra_bits = (length - 31);
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
			bitstream_big_endian_insert(stream, extra_bits, 2);
		}
			break;
		case 35:case 36:case 37:case 38:case 39:case 40:case 41:case 42:
		/* code 273 3 extra bits */
		{
			unsigned char first_byte = ((unsigned int) 273) - 256;
			unsigned char extra_bits = (length - 35);
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
			bitstream_big_endian_insert(stream, extra_bits, 3);
		}
			break;
		case 43:case 44:case 45:case 46:case 47:case 48:case 49:case 50:
		/* code 274 3 extra bits */
		{
			unsigned char first_byte = ((unsigned int) 274) - 256;
			unsigned char extra_bits = (length - 43);
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
			bitstream_big_endian_insert(stream, extra_bits, 3);
		}
			break;
		case 51:case 52:case 53:case 54:case 55:case 56:case 57:case 58:
		/* code 275 3 extra bits */
		{
			unsigned char first_byte = ((unsigned int) 275) - 256;
			unsigned char extra_bits = (length - 51);
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
			bitstream_big_endian_insert(stream, extra_bits, 3);
		}
			break;
		case 59:case 60:case 61:case 62:case 63:case 64:case 65:case 66:
		/* code 276 3 extra bits */
		{
			unsigned char first_byte = ((unsigned int) 276) - 256;
			unsigned char extra_bits = (length - 59);
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
			bitstream_big_endian_insert(stream, extra_bits, 3);
		}
			break;
		case 67:case 68:case 69:case 70:case 71:case 72:case 73:case 74:case 75:case 76:case 77:case 78:case 79:case 80:case 81:case 82:
		/* code 277 4 extra bits */
		{
			unsigned char first_byte = ((unsigned int) 277) - 256;
			unsigned char extra_bits = (length - 67);
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
			bitstream_big_endian_insert(stream, extra_bits, 4);
		}
			break;
		case 83:case 84:case 85:case 86:case 87:case 88:case 89:case 90:case 91:case 92:case 93:case 94:case 95:case 96:case 97:case 98:
		/* code 278 4 extra bits */
		{
			unsigned char first_byte = ((unsigned int) 278) - 256;
			unsigned char extra_bits = (length - 83);
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
			bitstream_big_endian_insert(stream, extra_bits, 4);
		}	
			break;
		case 99:case 100:case 101:case 102:case 103:case 104:case 105:case 106:case 107:case 108:case 109:case 110:case 111:case 112:case 113:case 114:
		/* code 279 4 extra bits */
		{
			unsigned char first_byte = ((unsigned int) 279) - 256;
			unsigned char extra_bits = (length - 99);
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
			bitstream_big_endian_insert(stream, extra_bits, 4);
		}
			break;
		case 115:case 116:case 117:case 118:case 119:case 120:case 121:case 122:case 123:case 124:case 125:case 126:case 127:case 128:case 129:case 130:
		/* code 280 4 extra bits */
		{
			unsigned char first_byte = ((unsigned int) 280) - 256;
			unsigned char extra_bits = (length - 115);
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
			bitstream_big_endian_insert(stream, extra_bits, 4);
		}
			break;
		case 131:case 132:case 133:case 134:case 135:case 136:case 137:case 138:case 139:case 140:case 141:case 142:case 143:case 144:case 145:case 146:case 147:case 148:case 149:case 150:case 151:case 152:case 153:case 154:case 155:case 156:case 157:case 158:case 159:case 160:case 161:case 162:
		/* code 281 5 extra bits */
		{
			unsigned char first_byte = ((unsigned int) 281) - 256;
			unsigned char extra_bits = (length - 131);
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
			bitstream_big_endian_insert(stream, extra_bits, 5);
		}

			break;
		case 163:case 164:case 165:case 166:case 167:case 168:case 169:case 170:case 171:case 172:case 173:case 174:case 175:case 176:case 177:case 178:case 179:case 180:case 181:case 182:case 183:case 184:case 185:case 186:case 187:case 188:case 189:case 190:case 191:case 192:case 193:case 194:
		/* code 282 5 extra bits */
		{
			unsigned char first_byte = ((unsigned int) 282) - 256;
			unsigned char extra_bits = (length - 163);
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
			bitstream_big_endian_insert(stream, extra_bits, 5);
		}
			break;
		case 195:case 196:case 197:case 198:case 199:case 200:case 201:case 202:case 203:case 204:case 205:case 206:case 207:case 208:case 209:case 210:case 211:case 212:case 213:case 214:case 215:case 216:case 217:case 218:case 219:case 220:case 221:case 222:case 223:case 224:case 225:case 226:

		/* code 283 5 extra bits */
		{
			unsigned char first_byte = ((unsigned int) 283) - 256;
			unsigned char extra_bits = (length - 195);
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
			bitstream_big_endian_insert(stream, extra_bits, 5);
		}
			break;
		case 227:case 228:case 229:case 230:case 231:case 232:case 233:case 234:case 235:case 236:case 237:case 238:case 239:case 240:case 241:case 242:case 243:case 244:case 245:case 246:case 247:case 248:case 249:case 250:case 251:case 252:case 253:case 254:case 255:case 256:case 257:
		/* code 284 5 extra bits */
		{
			unsigned char first_byte = (unsigned char)((284) - 256);
			unsigned char extra_bits = (length - 227);
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
		        bitstream_big_endian_insert(stream, extra_bits, 5);
		}
			break;
		case 258:
		{
			unsigned char first_byte = ((unsigned int) 285) - 256;
			bitstream_insert(stream, first_byte, 8);
			bitstream_insert(stream, 1, 1);
		}
			break;
		default:
			assert(0);
			break;
	}
}
