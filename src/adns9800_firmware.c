// the firmeware that is uploaded in the ADNS each time it boots
#include <avr/pgmspace.h>

// Firmware "adns9800_srom_A4.txt" from 
// This firmware is Copyright Avago, please refer to them concerning modifications.


prog_uchar firmware_data[] PROGMEM = {
0x03, 0xa6, 0x68, 0x1e, 0x7d, 0x10, 0x7e, 0x7e, 0x5f, 0x1c, 0xb8, 0xf2, 0x47, 0x0c, 0x7b,
0x74, 0x4b, 0x14, 0x8b, 0x75, 0x66, 0x51, 0x0b, 0x8c, 0x76, 0x74, 0x4b, 0x14, 0xaa, 0xd6,
0x0f, 0x9c, 0xba, 0xf6, 0x6e, 0x3f, 0xdd, 0x38, 0xd5, 0x02, 0x80, 0x9b, 0x82, 0x6d, 0x58,
0x13, 0xa4, 0xab, 0xb5, 0xc9, 0x10, 0xa2, 0xc6, 0x0a, 0x7f, 0x5d, 0x19, 0x91, 0xa0, 0xa3,
0xce, 0xeb, 0x3e, 0xc9, 0xf1, 0x60, 0x42, 0xe7, 0x4c, 0xfb, 0x74, 0x6a, 0x56, 0x2e, 0xbf,
0xdd, 0x38, 0xd3, 0x05, 0x88, 0x92, 0xa6, 0xce, 0xff, 0x5d, 0x38, 0xd1, 0xcf, 0xef, 0x58,
0xcb, 0x65, 0x48, 0xf0, 0x35, 0x85, 0xa9, 0xb2, 0x8f, 0x5e, 0xf3, 0x80, 0x94, 0x97, 0x7e,
0x75, 0x97, 0x87, 0x73, 0x13, 0xb0, 0x8a, 0x69, 0xd4, 0x0a, 0xde, 0xc1, 0x79, 0x59, 0x36,
0xdb, 0x9d, 0xd6, 0xb8, 0x15, 0x6f, 0xce, 0x3c, 0x72, 0x32, 0x45, 0x88, 0xdf, 0x6c, 0xa5,
0x6d, 0xe8, 0x76, 0x96, 0x14, 0x74, 0x20, 0xdc, 0xf4, 0xfa, 0x37, 0x6a, 0x27, 0x32, 0xe3,
0x29, 0xbf, 0xc4, 0xc7, 0x06, 0x9d, 0x58, 0xe7, 0x87, 0x7c, 0x2e, 0x9f, 0x6e, 0x49, 0x07,
0x5d, 0x23, 0x64, 0x54, 0x83, 0x6e, 0xcb, 0xb7, 0x77, 0xf7, 0x2b, 0x6e, 0x0f, 0x2e, 0x66,
0x12, 0x60, 0x55, 0x65, 0xfc, 0x43, 0xb3, 0x58, 0x73, 0x5b, 0xe8, 0x67, 0x04, 0x43, 0x02,
0xde, 0xb3, 0x89, 0xa0, 0x6d, 0x3a, 0x27, 0x79, 0x64, 0x5b, 0x0c, 0x16, 0x9e, 0x66, 0xb1,
0x8b, 0x87, 0x0c, 0x5d, 0xf2, 0xb6, 0x3d, 0x71, 0xdf, 0x42, 0x03, 0x8a, 0x06, 0x8d, 0xef,
0x1d, 0xa8, 0x96, 0x5c, 0xed, 0x31, 0x61, 0x5c, 0xa1, 0x34, 0xf6, 0x8c, 0x08, 0x60, 0x33,
0x07, 0x00, 0x3e, 0x79, 0x95, 0x1b, 0x43, 0x7f, 0xfe, 0xb6, 0xa6, 0xd4, 0x9d, 0x76, 0x72,
0xbf, 0xad, 0xc0, 0x15, 0xe8, 0x37, 0x31, 0xa3, 0x72, 0x63, 0x52, 0x1d, 0x1c, 0x5d, 0x51,
0x1b, 0xe1, 0xa9, 0xed, 0x60, 0x32, 0x3e, 0xa9, 0x50, 0x28, 0x53, 0x06, 0x59, 0xe2, 0xfc,
0xe7, 0x02, 0x64, 0x39, 0x21, 0x56, 0x4a, 0xa5, 0x40, 0x80, 0x81, 0xd5, 0x5a, 0x60, 0x7b,
0x68, 0x84, 0xf1, 0xe0, 0xb1, 0xb6, 0x5b, 0xdf, 0xa8, 0x1d, 0x6d, 0x65, 0x20, 0xc0, 0xa2,
0xb9, 0xd9, 0xbb, 0x00, 0xa6, 0xdb, 0x8b, 0x01, 0x53, 0x91, 0xfe, 0xc4, 0x51, 0x85, 0xb0,
0x96, 0x7f, 0xfd, 0x51, 0xdd, 0x14, 0x03, 0x67, 0x2e, 0x75, 0x1c, 0x76, 0xd3, 0x6e, 0xdd,
0x99, 0x55, 0x76, 0xe5, 0xab, 0x23, 0xfc, 0x4a, 0xd5, 0xc6, 0xe8, 0x2e, 0xca, 0x8a, 0xb3,
0xf6, 0x8c, 0x6c, 0xb0, 0xe9, 0xf2, 0xe7, 0x9e, 0x69, 0x41, 0xed, 0xf1, 0x6d, 0xd2, 0x86,
0xd8, 0x7e, 0xcb, 0x5d, 0x47, 0x6c, 0x85, 0x6a, 0x23, 0xed, 0x20, 0x40, 0x93, 0xb4, 0x20,
0xc7, 0xa5, 0xc9, 0xaf, 0x03, 0x15, 0xac, 0x19, 0xe5, 0x2a, 0x36, 0xdf, 0x6d, 0xc5, 0x8c,
0x80, 0x07, 0xce, 0x92, 0x0c, 0xd8, 0x06, 0x62, 0x0f, 0xdd, 0x48, 0x46, 0x1a, 0x53, 0xc7,
0x8a, 0x8c, 0x5d, 0x5d, 0xb4, 0xa1, 0x02, 0xd3, 0xa9, 0xb8, 0xf3, 0x94, 0x8f, 0x3f, 0xe5,
0x54, 0xd4, 0x11, 0x65, 0xb2, 0x5e, 0x09, 0x0b, 0x81, 0xe3, 0x75, 0xa7, 0x89, 0x81, 0x39,
0x6c, 0x46, 0xf6, 0x06, 0x9f, 0x27, 0x3b, 0xb6, 0x2d, 0x5f, 0x1d, 0x4b, 0xd4, 0x7b, 0x1d,
0x61, 0x74, 0x89, 0xe4, 0xe3, 0xbd, 0x98, 0x1b, 0xc4, 0x51, 0x3b, 0xa4, 0xfa, 0xe0, 0x92,
0xf7, 0xbe, 0xf2, 0x4d, 0xbb, 0xff, 0xad, 0x4f, 0x6d, 0x68, 0xc2, 0x79, 0x40, 0xaa, 0x9b,
0x8f, 0x0c, 0x32, 0x4b, 0x5f, 0x3e, 0xab, 0x59, 0x98, 0xb3, 0xf5, 0x1d, 0xac, 0x5e, 0xbc,
0x78, 0xd3, 0x01, 0x6c, 0x64, 0x15, 0x2f, 0xd8, 0x71, 0xa6, 0x2d, 0x45, 0xe1, 0x22, 0x42,
0xe4, 0x4e, 0x04, 0x3c, 0x7d, 0xf4, 0x40, 0x21, 0xb4, 0x67, 0x05, 0xa8, 0xe2, 0xf3, 0x72,
0x87, 0x4c, 0x7d, 0xd9, 0x1b, 0x65, 0x97, 0xf3, 0xc2, 0xe3, 0xe4, 0xc8, 0xd2, 0xde, 0xf6,
0xef, 0xdc, 0xbb, 0x44, 0x08, 0x5e, 0xe2, 0x45, 0x27, 0x01, 0xb0, 0xf6, 0x43, 0xe7, 0x3a,
0xf6, 0xdc, 0x9d, 0xed, 0xf3, 0xc5, 0x0c, 0xb8, 0x9c, 0x98, 0x3a, 0xd8, 0x36, 0xee, 0x96,
0x72, 0x67, 0xe7, 0x81, 0x91, 0xd5, 0x05, 0x0a, 0xe0, 0x82, 0xd5, 0x8f, 0xe8, 0xf9, 0xb0,
0xc9, 0xcf, 0x93, 0xe7, 0x04, 0xc5, 0xbc, 0x2b, 0x43, 0x56, 0x7e, 0xe8, 0x67, 0x7c, 0xe5,
0xfb, 0x49, 0xad, 0x5e, 0x9f, 0x25, 0x13, 0xde, 0x6e, 0x6e, 0xe9, 0xf1, 0xec, 0x87, 0x0b,
0x59, 0x81, 0x76, 0x84, 0x76, 0xb3, 0x24, 0xaf, 0x30, 0xfd, 0x27, 0x8b, 0xab, 0xd8, 0x00,
0x8b, 0x9b, 0x0c, 0xd2, 0xb2, 0x4e, 0x5e, 0x9d, 0x1d, 0x96, 0x01, 0x00, 0x67, 0xc1, 0x5f,
0x02, 0x20, 0xfd, 0x45, 0x6a, 0x01, 0x60, 0x58, 0x45, 0xca, 0x47, 0x21, 0x90, 0x5a, 0xc4,
0x43, 0x26, 0x1a, 0xd7, 0xa5, 0x4a, 0xb2, 0x5d, 0x2b, 0x35, 0x49, 0xfb, 0xa5, 0x17, 0x92,
0x21, 0x1e, 0x93, 0x96, 0x67, 0xa2, 0x7e, 0x36, 0x7a, 0xde, 0x5f, 0xbe, 0x7a, 0x58, 0x9d,
0xf8, 0x78, 0xa3, 0xfa, 0xc8, 0xd5, 0x17, 0xf0, 0x21, 0x97, 0x8c, 0x80, 0xb5, 0x4b, 0x3b,
0xbd, 0xbb, 0x41, 0x21, 0xa8, 0x50, 0x67, 0xf7, 0xe7, 0x19, 0x80, 0x10, 0x8e, 0xce, 0x04,
0x18, 0x3f, 0x51, 0x6b, 0x77, 0xd8, 0x9e, 0x16, 0xaf, 0xec, 0xef, 0x48, 0x16, 0x4d, 0x9e,
0x85, 0x38, 0x18, 0x3e, 0xd4, 0x28, 0x87, 0x60, 0x2a, 0xf6, 0x7f, 0x09, 0x86, 0x6f, 0x9c,
0x3c, 0x3a, 0xff, 0xab, 0xd0, 0x61, 0xa2, 0x97, 0x0d, 0x71, 0x94, 0x7e, 0xfd, 0xb9, 0x80,
0x02, 0x89, 0x6a, 0xb3, 0x84, 0x6c, 0x2a, 0x77, 0x62, 0xbe, 0x0b, 0xf4, 0xaf, 0xac, 0x7b,
0x7c, 0x8e, 0xca, 0x01, 0xba, 0x71, 0x78, 0x94, 0xfd, 0xb5, 0x39, 0xa4, 0x4d, 0x2f, 0x78,
0xcf, 0xca, 0x92, 0x0c, 0x1a, 0x99, 0x48, 0x4c, 0x11, 0x96, 0xb5, 0x4e, 0x41, 0x28, 0xe4,
0xa6, 0xfe, 0x4b, 0x72, 0x91, 0xe7, 0xd4, 0xdd, 0x9f, 0x12, 0xe6, 0x29, 0x38, 0xce, 0x45,
0xae, 0x02, 0xb8, 0x24, 0xae, 0xbd, 0xe9, 0x66, 0x08, 0x62, 0xa2, 0x2c, 0x2b, 0x00, 0xe2,
0x23, 0xd9, 0xc4, 0x48, 0xe4, 0xd3, 0xac, 0xbb, 0x34, 0xc7, 0xf0, 0xe3, 0x4f, 0xb9, 0x30,
0xea, 0xa2, 0x12, 0xf1, 0x30, 0x2c, 0x36, 0xde, 0x48, 0xf2, 0xb0, 0x4c, 0x43, 0x3f, 0x2e,
0x58, 0xe4, 0x20, 0xe3, 0x58, 0xcd, 0x31, 0x22, 0xf0, 0xa2, 0x2a, 0xe6, 0x19, 0x90, 0x55,
0x86, 0xf6, 0x55, 0x79, 0xd1, 0xd7, 0x46, 0x2f, 0xc0, 0xdc, 0x99, 0xe8, 0xf3, 0x6a, 0xdf,
0x7f, 0xeb, 0x24, 0x4a, 0x1e, 0x5a, 0x75, 0xde, 0x2f, 0x5c, 0x19, 0x61, 0x03, 0x53, 0x54,
0x6a, 0x3b, 0x18, 0x70, 0xb6, 0x4f, 0xf1, 0x9c, 0x0a, 0x59, 0x9d, 0x19, 0x92, 0x65, 0x8c,
0x83, 0x14, 0x2d, 0x44, 0x8a, 0x75, 0xa9, 0xf5, 0x90, 0xd2, 0x66, 0x4e, 0xfa, 0x69, 0x0f,
0x5b, 0x0b, 0x98, 0x65, 0xc8, 0x11, 0x42, 0x59, 0x7f, 0xdd, 0x1b, 0x75, 0x17, 0x31, 0x4c,
0x75, 0x58, 0xeb, 0x58, 0x63, 0x7d, 0xf2, 0xa6, 0xc2, 0x6e, 0xb7, 0x3f, 0x3e, 0x5e, 0x47,
0xad, 0xb7, 0x04, 0xe8, 0x05, 0xf8, 0xb2, 0xcf, 0x19, 0xf3, 0xd2, 0x85, 0xfe, 0x3e, 0x3e,
0xb1, 0x62, 0x08, 0x2c, 0x10, 0x07, 0x0d, 0x73, 0x90, 0x17, 0xfa, 0x9b, 0x56, 0x02, 0x75,
0xf9, 0x51, 0xe0, 0xe9, 0x1a, 0x7b, 0x9f, 0xb3, 0xf3, 0x98, 0xb8, 0x1c, 0x9c, 0xe1, 0xd5,
0x35, 0xae, 0xc8, 0x60, 0x48, 0x11, 0x09, 0x94, 0x6b, 0xd0, 0x8b, 0x15, 0xbc, 0x05, 0x68,
0xd3, 0x54, 0x8a, 0x51, 0x39, 0x5c, 0x42, 0x76, 0xce, 0xd8, 0xad, 0x89, 0x30, 0xc9, 0x05,
0x1c, 0xcc, 0x94, 0x3f, 0x0f, 0x90, 0x6f, 0x72, 0x2d, 0x85, 0x64, 0x9a, 0xb9, 0x23, 0xf9,
0x0b, 0xc3, 0x7c, 0x39, 0x0f, 0x97, 0x07, 0x97, 0xda, 0x58, 0x48, 0x33, 0x05, 0x23, 0xb8,
0x82, 0xe8, 0xd3, 0x53, 0x89, 0xaf, 0x33, 0x80, 0x22, 0x84, 0x0c, 0x95, 0x5c, 0x67, 0xb8,
0x77, 0x0c, 0x5c, 0xa2, 0x5f, 0x3d, 0x58, 0x0f, 0x27, 0xf3, 0x2f, 0xae, 0x48, 0xbd, 0x0b,
0x6f, 0x54, 0xfb, 0x67, 0x4c, 0xea, 0x32, 0x27, 0xf1, 0xfa, 0xe2, 0xb0, 0xec, 0x0b, 0x15,
0xb4, 0x70, 0xf6, 0x5c, 0xdd, 0x71, 0x60, 0xc3, 0xc1, 0xa8, 0x32, 0x65, 0xac, 0x7a, 0x77,
0x41, 0xe5, 0xa9, 0x6b, 0x11, 0x81, 0xfa, 0x34, 0x8d, 0xfb, 0xc1, 0x80, 0x6e, 0xc4, 0x60,
0x30, 0x07, 0xd4, 0x8b, 0x67, 0xbd, 0xaa, 0x8c, 0x9c, 0x64, 0xac, 0xdb, 0x0b, 0x24, 0x8b,
0x63, 0x6f, 0xe6, 0xbc, 0xe7, 0x33, 0xa4, 0x4a, 0x4c, 0xa7, 0x9f, 0x43, 0x53, 0xd2, 0xbb,
0x8f, 0x43, 0xc7, 0x3d, 0x78, 0x68, 0x3f, 0xa5, 0x3d, 0xca, 0x69, 0x84, 0xa6, 0x97, 0x2d,
0xc0, 0x7d, 0x31, 0x34, 0x55, 0x1d, 0x07, 0xb1, 0x5f, 0x40, 0x5c, 0x93, 0xb0, 0xbc, 0x7c,
0xb0, 0xbc, 0xe7, 0x12, 0xee, 0x6b, 0x2b, 0xd3, 0x4d, 0x67, 0x70, 0x3a, 0x9a, 0xf2, 0x3c,
0x7c, 0x81, 0xfa, 0xd7, 0xd9, 0x90, 0x91, 0x81, 0xb8, 0xb1, 0xf3, 0x48, 0x6a, 0x26, 0x4f,
0x0c, 0xce, 0xb0, 0x9e, 0xfd, 0x4a, 0x3a, 0xaf, 0xac, 0x5b, 0x3f, 0xbf, 0x44, 0x5a, 0xa3,
0x19, 0x1e, 0x4b, 0xe7, 0x36, 0x6a, 0xd7, 0x20, 0xae, 0xd7, 0x7d, 0x3b, 0xe7, 0xff, 0x3a,
0x86, 0x2e, 0xd0, 0x4a, 0x3e, 0xaf, 0x9f, 0x8e, 0x01, 0xbf, 0xf8, 0x4f, 0xc1, 0xe8, 0x6f,
0x74, 0xe1, 0x45, 0xd3, 0xf7, 0x04, 0x6a, 0x4b, 0x9d, 0xec, 0x33, 0x27, 0x76, 0xd7, 0xc5,
0xe1, 0xb0, 0x3b, 0x0e, 0x23, 0xec, 0xf0, 0x86, 0xd2, 0x1a, 0xbf, 0x3d, 0x04, 0x62, 0xb3,
0x6c, 0xb2, 0xeb, 0x17, 0x05, 0xa6, 0x0a, 0x8a, 0x7e, 0x83, 0x1c, 0xb6, 0x37, 0x09, 0xc6,
0x0b, 0x70, 0x3c, 0xb5, 0x93, 0x81, 0xd8, 0x93, 0xa0, 0x5f, 0x1e, 0x08, 0xe2, 0xc6, 0xe5,
0xc9, 0x72, 0xf1, 0xf1, 0xc1, 0xed, 0xd5, 0x58, 0x93, 0x83, 0xf8, 0x65, 0x67, 0x2e, 0x0d,
0xa9, 0xf1, 0x64, 0x12, 0xe6, 0x4c, 0xea, 0x15, 0x3f, 0x8c, 0x1a, 0xb6, 0xbf, 0xf6, 0xb9,
0x52, 0x35, 0x09, 0xb0, 0xe6, 0xf7, 0xcd, 0xf1, 0xa5, 0xaa, 0x81, 0xd1, 0x81, 0x6f, 0xb4,
0xa9, 0x66, 0x1f, 0xfc, 0x48, 0xc0, 0xb6, 0xd1, 0x8b, 0x06, 0x2f, 0xf6, 0xef, 0x1f, 0x0a,
0xe6, 0xce, 0x3a, 0x4a, 0x55, 0xbf, 0x6d, 0xf9, 0x4d, 0xd4, 0x08, 0x45, 0x4b, 0xc3, 0x66,
0x19, 0x92, 0x10, 0xe1, 0x17, 0x8e, 0x28, 0x91, 0x16, 0xbf, 0x3c, 0xee, 0xa3, 0xa6, 0x99,
0x92, 0x10, 0xe1, 0xf6, 0xcc, 0xac, 0xb8, 0x65, 0x0b, 0x43, 0x66, 0xf8, 0xe3, 0xe5, 0x3f,
0x24, 0x89, 0x47, 0x5d, 0x78, 0x43, 0xd0, 0x61, 0x17, 0xbd, 0x5b, 0x64, 0x54, 0x08, 0x45,
0x59, 0x93, 0xf6, 0x95, 0x8a, 0x41, 0x51, 0x62, 0x4b, 0x51, 0x02, 0x30, 0x73, 0xc7, 0x87,
0xc5, 0x4b, 0xa2, 0x97, 0x0f, 0xe8, 0x46, 0x5f, 0x7e, 0x2a, 0xe1, 0x30, 0x20, 0xb0, 0xfa,
0xe7, 0xce, 0x61, 0x42, 0x57, 0x6e, 0x21, 0xf3, 0x7a, 0xec, 0xe3, 0x25, 0xc7, 0x25, 0xf3,
0x67, 0xa7, 0x57, 0x40, 0x00, 0x02, 0xcf, 0x1c, 0x80, 0x77, 0x67, 0xbd, 0x70, 0xa1, 0x19,
0x92, 0x31, 0x75, 0x93, 0x27, 0x27, 0xb6, 0x82, 0xe4, 0xeb, 0x1d, 0x78, 0x48, 0xe7, 0xa5,
0x5e, 0x57, 0xef, 0x64, 0x28, 0x64, 0x1b, 0xf6, 0x11, 0xb2, 0x03, 0x9d, 0xb9, 0x18, 0x02,
0x27, 0xf7, 0xbe, 0x9d, 0x55, 0xfc, 0x00, 0xd2, 0xc7, 0xae, 0xad, 0x0b, 0xc5, 0xe9, 0x42,
0x41, 0x48, 0xd8, 0x32, 0xcf, 0xf6, 0x0f, 0xf5, 0xbc, 0x97, 0xc6, 0x99, 0x47, 0x76, 0xbd,
0x89, 0x06, 0x0f, 0x63, 0x0c, 0x51, 0xd4, 0x5e, 0xea, 0x48, 0xa8, 0xa2, 0x56, 0x1c, 0x79,
0x84, 0x86, 0x40, 0x88, 0x41, 0x76, 0x55, 0xfc, 0xc2, 0xd7, 0xfd, 0xc9, 0xc7, 0x80, 0x61,
0x35, 0xa7, 0x43, 0x20, 0xf7, 0xeb, 0x6c, 0x66, 0x13, 0xb0, 0xec, 0x02, 0x75, 0x3e, 0x4b,
0xaf, 0xb9, 0x5d, 0x40, 0xda, 0xd6, 0x6e, 0x2d, 0x39, 0x54, 0xc2, 0x95, 0x35, 0x54, 0x25,
0x72, 0xe1, 0x78, 0xb8, 0xeb, 0xc1, 0x16, 0x58, 0x0f, 0x9c, 0x9b, 0xb4, 0xea, 0x37, 0xec,
0x3b, 0x11, 0xba, 0xd5, 0x8a, 0xa9, 0xe3, 0x98, 0x00, 0x51, 0x1c, 0x14, 0xe0, 0x40, 0x96,
0xe5, 0xe9, 0xf2, 0x21, 0x22, 0xb1, 0x23, 0x60, 0x78, 0xd3, 0x17, 0xf8, 0x7a, 0xa5, 0xa8,
0xba, 0x20, 0xd3, 0x15, 0x1e, 0x32, 0xe4, 0x5e, 0x15, 0x48, 0xae, 0xa9, 0xe5, 0xb8, 0x33,
0xec, 0xe8, 0xa2, 0x42, 0xac, 0xbf, 0x10, 0x84, 0x53, 0x87, 0x19, 0xb4, 0x5f, 0x76, 0x4d,
0x01, 0x9d, 0x56, 0x74, 0xd9, 0x5c, 0x97, 0xe7, 0x88, 0xea, 0x3a, 0xbf, 0xdc, 0x4c, 0x33,
0x8a, 0x16, 0xb9, 0x5b, 0xfa, 0xd8, 0x42, 0xa7, 0xbb, 0x3c, 0x04, 0x27, 0x78, 0x49, 0x81,
0x2a, 0x5a, 0x7d, 0x7c, 0x23, 0xa8, 0xba, 0xf7, 0x9a, 0x9f, 0xd2, 0x66, 0x3e, 0x38, 0x3c,
0x75, 0xf9, 0xd1, 0x30, 0x26, 0x30, 0x6e, 0x5a, 0x6e, 0xdc, 0x6a, 0x69, 0x32, 0x50, 0x33,
0x47, 0x9e, 0xa4, 0xa8, 0x64, 0x66, 0xf0, 0x8a, 0xe4, 0xfd, 0x27, 0x6f, 0x51, 0x25, 0x8b,
0x43, 0x74, 0xc9, 0x8e, 0xbd, 0x88, 0x31, 0xbe, 0xec, 0x65, 0xd2, 0xcb, 0x8d, 0x5a, 0x13,
0x48, 0x16, 0x8c, 0x61, 0x0b, 0x11, 0xf6, 0xc6, 0x66, 0xae, 0xc3, 0xcc, 0x0c, 0xd2, 0xe1,
0x9f, 0x82, 0x41, 0x3f, 0x56, 0xf9, 0x73, 0xef, 0xdc, 0x30, 0x50, 0xcf, 0xb6, 0x7f, 0xbc,
0xd0, 0xb3, 0x10, 0xab, 0x24, 0xe4, 0xec, 0xad, 0x18, 0x8c, 0x39, 0x2d, 0x30, 0x4c, 0xc5,
0x40, 0x0d, 0xf6, 0xac, 0xd6, 0x18, 0x5d, 0x96, 0xbf, 0x5f, 0x71, 0x75, 0x96, 0x22, 0x97,
0x0f, 0x02, 0x94, 0x6e, 0xa6, 0xae, 0x6d, 0x8f, 0x1e, 0xca, 0x12, 0x9b, 0x2a, 0x1c, 0xce,
0xa9, 0xee, 0xfd, 0x12, 0x8e, 0xfc, 0xed, 0x09, 0x33, 0xba, 0xf4, 0x1a, 0x15, 0xf6, 0x9d,
0x87, 0x16, 0x43, 0x7c, 0x78, 0x57, 0xe1, 0x44, 0xc9, 0xeb, 0x1f, 0x58, 0x4d, 0xc1, 0x49,
0x11, 0x5c, 0xb2, 0x11, 0xa8, 0x55, 0x16, 0xf1, 0xc6, 0x50, 0xe9, 0x87, 0x89, 0xf6, 0xcf,
0xd8, 0x9c, 0x51, 0xa7, 0xbc, 0x5b, 0x31, 0x6d, 0x4d, 0x51, 0xd0, 0x4c, 0xbc, 0x0d, 0x58,
0x2d, 0x7b, 0x88, 0x7a, 0xf9, 0x8e, 0xd6, 0x40, 0x4d, 0xbb, 0xbe, 0xc4, 0xe5, 0x07, 0xfc,
0xd9, 0x7b, 0x6d, 0xa6, 0x42, 0x57, 0x8f, 0x02, 0x94, 0x4f, 0xe4, 0x2a, 0x65, 0xe2, 0x19,
0x5a, 0x50, 0xe1, 0x25, 0x65, 0x4a, 0x60, 0xc2, 0xcd, 0xa8, 0xec, 0x05, 0x2e, 0x87, 0x7b,
0x95, 0xb7, 0x4f, 0xa0, 0x0b, 0x1b, 0x4a, 0x7f, 0x92, 0xc8, 0x90, 0xee, 0x89, 0x1e, 0x10,
0xd2, 0x85, 0xe4, 0x9f, 0x63, 0xc8, 0x12, 0xbb, 0x4e, 0xb8, 0xcf, 0x0a, 0xec, 0x18, 0x4e,
0xe6, 0x7c, 0xb3, 0x33, 0x26, 0xc7, 0x1f, 0xd2, 0x04, 0x23, 0xea, 0x07, 0x0c, 0x5f, 0x90,
0xbd, 0xa7, 0x6a, 0x0f, 0x4a, 0xd6, 0x10, 0x01, 0x3c, 0x12, 0x29, 0x2e, 0x96, 0xc0, 0x4d,
0xbb, 0xbe, 0xe5, 0xa7, 0x83, 0xd5, 0x6a, 0x3c, 0xe3, 0x5b, 0xb8, 0xf2, 0x5c, 0x6d, 0x1f,
0xa6, 0xf3, 0x12, 0x24, 0xf6, 0xd6, 0x3b, 0x10, 0x14, 0x09, 0x07, 0x82, 0xe8, 0x30, 0x6a,
0x99, 0xdc, 0x95, 0x01, 0x9c, 0xd4, 0x68, 0x3b, 0xca, 0x98, 0x12, 0xab, 0x77, 0x25, 0x15,
0x7d, 0x10, 0x32, 0x45, 0x98, 0xcd, 0x7a, 0xdf, 0x71, 0x8a, 0x75, 0xc1, 0x1c, 0xd4, 0x68,
0x25, 0xeb, 0xbb, 0x54, 0x27, 0x6f, 0x2a, 0xf7, 0xb9, 0x98, 0x03, 0x27, 0xde, 0x24, 0xa8,
0xbb, 0x98, 0xc2, 0x84, 0xff, 0x9b, 0x51, 0xd8, 0x53, 0x50, 0xda, 0xf5, 0x88, 0xaa, 0x87,
0x2f, 0xae, 0xd6, 0xea, 0x6b, 0xde, 0xc8, 0xd7, 0xa7, 0x28, 0x65, 0x81, 0xe8, 0xb2, 0x3b,
0x1d, 0x4f, 0x75, 0x8f, 0x9f, 0x7a, 0x74, 0x8e, 0xc1, 0x5f, 0x9a, 0xa8, 0x9d, 0xfa, 0x03,
0xa3, 0x71, 0x9b, 0x37, 0x6d, 0xd5, 0x0b, 0xf5, 0xe1, 0xa1, 0x1b, 0x01, 0x6a, 0xc6, 0x67,
0xaa, 0xea, 0x2c, 0x9d, 0xa4, 0xd2, 0x6e, 0xfc, 0xde, 0x2e, 0x7f, 0x94, 0x69, 0xe5, 0x4a,
0xe0, 0x01, 0x48, 0x3c, 0x6b, 0xf7, 0x1e, 0xb6, 0x0b, 0x5f, 0xf9, 0x2e, 0x07, 0xc5, 0xe8,
0xae, 0x37, 0x1b, 0xbc, 0x3c, 0xd8, 0xd5, 0x0b, 0x91, 0x9e, 0x80, 0x24, 0xf5, 0x06, 0x0c,
0x0e, 0x98, 0x07, 0x96, 0x2d, 0x19, 0xdc, 0x58, 0x93, 0xcc, 0xfb, 0x4e, 0xeb, 0xbd, 0x0f,
0xf5, 0xaf, 0x01, 0xfa, 0xf1, 0x7c, 0x43, 0x8c, 0xb8, 0x56, 0x3e, 0xbe, 0x77, 0x4e, 0x2b,
0xf7, 0xbb, 0xb7, 0x45, 0x47, 0xcd, 0xcc, 0xa6, 0x4c, 0x72, 0x7b, 0x6a, 0x2a, 0x70, 0x13,
0x07, 0xfd, 0xb8, 0x9c, 0x98, 0x3a, 0xd8, 0x23, 0x67, 0x5b, 0x34, 0xd5, 0x14, 0x0c, 0xab,
0x77, 0x1f, 0xf8, 0x3d, 0x5a, 0x9f, 0x92, 0xb7, 0x2c, 0xad, 0x31, 0xde, 0x61, 0x07, 0xb3,
0x6b, 0xf7, 0x38, 0x15, 0x95, 0x46, 0x14, 0x48, 0x53, 0x69, 0x52, 0x66, 0x07, 0x6d, 0x83,
0x71, 0x8a, 0x67, 0x25, 0x20, 0x0f, 0xfe, 0xd7, 0x02, 0xd7, 0x6e, 0x2c, 0xd2, 0x1a, 0x0a,
0x5d, 0xfd, 0x0f, 0x74, 0xe3, 0xa4, 0x36, 0x07, 0x9a, 0xdf, 0xd4, 0x79, 0xbf, 0xef, 0x59,
0xc0, 0x44, 0x52, 0x87, 0x9a, 0x6e, 0x1d, 0x0e, 0xee, 0xde, 0x2e, 0x1a, 0xa9, 0x8f, 0x3a,
0xc9, 0xba, 0xec, 0x99, 0x78, 0x2d, 0x55, 0x6b, 0x14, 0xc2, 0x06, 0xd5, 0xfc, 0x93, 0x53,
0x4d, 0x11, 0x8c, 0xf8, 0xfa, 0x79, 0x7c, 0xa6, 0x64, 0xae, 0x61, 0xb8, 0x7b, 0x94, 0x56,
0xa6, 0x39, 0x78, 0x9a, 0xe5, 0xc7, 0xdf, 0x18, 0x63, 0x23, 0x9c, 0xfa, 0x66, 0xbb, 0xb7,
0x5a, 0x27, 0x4c, 0xd1, 0xa1, 0x83, 0x22, 0xb3, 0x52, 0x49, 0x35, 0xb0, 0x22, 0x83, 0x59,
0x12, 0x00, 0x16, 0x98, 0xdd, 0xad, 0xc2, 0x94, 0xf9, 0xd3, 0x7b, 0x64, 0x7f, 0x44, 0x3e,
0x3c, 0x8b, 0x9a, 0x83, 0x9c, 0x69, 0x6b, 0xe4, 0xdf, 0x9f, 0xed, 0x54, 0x1f, 0xe5, 0x5d,
0x7a, 0x05, 0x82, 0xb3, 0xdd, 0xef, 0xfc, 0x53, 0x96, 0xb0, 0x2c, 0x5a, 0xf8, 0xdf, 0x9c,
0x8b, 0x16, 0x4e, 0xdf, 0xda, 0x4d, 0x09, 0x09, 0x69, 0x50, 0x03, 0x65, 0xd8, 0x73, 0x70,
0xe8, 0x86, 0xbf, 0xbb, 0x35, 0xce, 0xb2, 0x46, 0xcb, 0x02, 0x00, 0x5b, 0xb4, 0xe2, 0xc6,
0x8f, 0x2f, 0x98, 0xaf, 0x87, 0x4b, 0x48, 0x45, 0xed, 0xcc, 0x1d, 0xe6, 0x58, 0xd6, 0xf2,
0x50, 0x25, 0x9f, 0x52, 0xc7, 0xcb, 0x8a, 0x17, 0x9d, 0x5b, 0xe5, 0xc8, 0xd7, 0x72, 0xb7,
0x52, 0xb2, 0xc4, 0x98, 0xe3, 0x7a, 0x17, 0x3e, 0xc6, 0x60, 0xa7, 0x97, 0xb0, 0xcf, 0x18,
0x81, 0x53, 0x84, 0x4c, 0xd5, 0x17, 0x32, 0x03, 0x13, 0x39, 0x51, 0x09, 0x10, 0xe3, 0x77,
0x49, 0x4f, 0x62, 0x01, 0xbf, 0x8c, 0x9a, 0xe0, 0x41, 0x9e, 0x89, 0x74, 0x36, 0xf9, 0x96,
0x86, 0x2e, 0x96, 0x1c, 0x4a, 0xb7, 0x2b, 0x4a, 0x97, 0xbc, 0x99, 0x40, 0xa3, 0xe0, 0x3d,
0xc8, 0xad, 0x2f, 0xdf, 0x4f, 0x2c, 0xc4, 0x69, 0x82, 0x9f, 0x9b, 0x81, 0x0c, 0x61, 0x5c,
0xa5, 0x9d, 0x8c, 0x89, 0xc0, 0x2c, 0xb4, 0x4a, 0x33, 0x4e, 0xeb, 0xa2, 0x56, 0x40, 0xc0,
0xc2, 0x46, 0xaf, 0x6a, 0xfc, 0x67, 0xd1, 0x80, 0x5e, 0xc5, 0x6d, 0x84, 0x43, 0x27, 0x3f,
0x55, 0x15, 0x96, 0x6a, 0xa0, 0xa5, 0xda, 0xb7, 0xff, 0xb7, 0x75, 0x6e, 0x4c, 0x49, 0x91,
0x9d, 0x22, 0xa3, 0x46, 0xea, 0xed, 0x9a, 0x00, 0xe2, 0x32, 0xc3, 0xd6, 0xa9, 0x71, 0x20,
0x55, 0xa3, 0x19, 0xed, 0xf8, 0x4f, 0xa7, 0x12, 0x9c, 0x66, 0x87, 0xaf, 0x4e, 0xb7, 0xf0,
0xdb, 0xbf, 0xef, 0xf0, 0xf6, 0xaf, 0xea, 0xda, 0x09, 0xfe, 0xde, 0x38, 0x5c, 0xa5, 0xa2,
0xdf, 0x99, 0x45, 0xa8, 0xe4, 0xe7, 0x92, 0xac, 0x67, 0xaa, 0x4f, 0xbf, 0x77, 0x3e, 0xa2,
0x40, 0x49, 0x22, 0x4a, 0x1e, 0x3b, 0xaa, 0x70, 0x7f, 0x95, 0xaf, 0x37, 0x4b, 0xfc, 0x99,
0xe2, 0xe0, 0xba, 0xd7, 0x34, 0xce, 0x55, 0x88, 0x5b, 0x84, 0x1b, 0x57, 0xc4, 0x80, 0x03,
0x53, 0xc9, 0x2f, 0x93, 0x04, 0x4d, 0xd5, 0x96, 0xe5, 0x70, 0xa6, 0x6e, 0x63, 0x5d, 0x9d,
0x6c, 0xdb, 0x02, 0x0a, 0xa9, 0xda, 0x8b, 0x53, 0xdc, 0xd9, 0x9a, 0xc5, 0x94, 0x2c, 0x91,
0x92, 0x2a, 0xde, 0xbb, 0x8b, 0x13, 0xb9, 0x19, 0x96, 0x64, 0xcc, 0xf2, 0x64, 0x39, 0xb7,
0x75, 0x49, 0xe9, 0x86, 0xc2, 0x86, 0x62, 0xd9, 0x24, 0xd3, 0x81, 0x35, 0x49, 0xfc, 0xa0,
0xa5, 0xa0, 0x93, 0x05, 0x64, 0xb4, 0x1a, 0x57, 0xce, 0x0c, 0x90, 0x02, 0x27, 0xc5, 0x7a,
0x2b, 0x5d, 0xae, 0x3e, 0xd5, 0xdd, 0x10, 0x7c, 0x14, 0xea, 0x3a, 0x08, 0xac, 0x72, 0x4e,
0x90, 0x3d, 0x3b, 0x7c, 0x86, 0x2e, 0xeb, 0xd4, 0x06, 0x70, 0xe6, 0xc7, 0xfb, 0x5f, 0xbd,
0x18, 0xf4, 0x11, 0xa4, 0x1a, 0x93, 0xc3, 0xbe, 0xd9, 0xfb, 0x26, 0x48, 0x2f, 0x37, 0x3c,
0xd0, 0x03, 0x47, 0x1a, 0xf7, 0x62, 0x19, 0x24, 0x5c, 0xf4, 0xa8, 0x92, 0x20, 0x7a, 0xf2,
0x9e, 0x2a, 0xc5, 0x95, 0xa2, 0xfb, 0xa4, 0xea, 0x85, 0xd8, 0x56, 0xb7, 0x70, 0xd1, 0x60,
0x30, 0xa5, 0x30, 0x82, 0x70, 0xdc, 0x7a, 0x65, 0x8a, 0x36, 0x3f, 0x5b, 0x0c, 0xae, 0x54,
0x7c, 0xd3, 0x57, 0x84, 0x7b, 0x3a, 0x65, 0x18, 0x81, 0xee, 0x05, 0x9b, 0x44, 0x4d, 0xb8,
0xda, 0xa2, 0xa1, 0xc9, 0x15, 0xd3, 0x73, 0x03, 0x0e, 0x43, 0xe9, 0x8e, 0x15, 0xf9, 0xbe,
0xc6, 0xc5, 0x8a, 0xe5, 0xc0, 0x1e, 0xc2, 0x37, 0x9e, 0x2a, 0x26, 0xa5, 0xa0, 0xbd, 0x24,
0x5f, 0xb9, 0xc1, 0xab, 0x34, 0x48, 0xb9, 0x5d, 0x98, 0xb4, 0x65, 0x18, 0xf3, 0x63, 0x19,
0x44, 0x1b, 0x11, 0x16, 0xff, 0xdc, 0xf1, 0x79, 0x08, 0x86, 0x0f, 0x52, 0x98, 0x73, 0xc4,
0x92, 0x90, 0x2b, 0x47, 0x09, 0xd0, 0x43, 0x6c, 0x2f, 0x20, 0xeb, 0xdc, 0xda, 0xc5, 0x08,
0x7b, 0x94, 0x42, 0x30, 0x6a, 0xc7, 0xda, 0x8c, 0xc3, 0x76, 0xa7, 0xa5, 0xcc, 0x62, 0x13,
0x00, 0x60, 0x31, 0x58, 0x44, 0x9b, 0xf5, 0x64, 0x14, 0xf5, 0x11, 0xc5, 0x54, 0x52, 0x83,
0xd4, 0x73, 0x01, 0x16, 0x0e, 0xb3, 0x7a, 0x29, 0x69, 0x35, 0x56, 0xd4, 0xee, 0x8a, 0x17,
0xa2, 0x99, 0x24, 0x9c, 0xd7, 0x8f, 0xdb, 0x55, 0xb5, 0x3e };
