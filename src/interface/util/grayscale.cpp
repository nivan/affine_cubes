#include "grayscale.h"

GrayScale::GrayScale() {
  _colors.resize(256);
  _colors[  0] = QColor(0, 0, 0);
  _colors[  1] = QColor(1, 1, 1);
  _colors[  2] = QColor(2, 2, 2);
  _colors[  3] = QColor(3, 3, 3);
  _colors[  4] = QColor(4, 4, 4);
  _colors[  5] = QColor(5, 5, 5);
  _colors[  6] = QColor(6, 6, 6);
  _colors[  7] = QColor(7, 7, 7);
  _colors[  8] = QColor(8, 8, 8);
  _colors[  9] = QColor(9, 9, 9);
  _colors[ 10] = QColor(10, 10, 10);
  _colors[ 11] = QColor(11, 11, 11);
  _colors[ 12] = QColor(12, 12, 12);
  _colors[ 13] = QColor(13, 13, 13);
  _colors[ 14] = QColor(14, 14, 14);
  _colors[ 15] = QColor(15, 15, 15);
  _colors[ 16] = QColor(16, 16, 16);
  _colors[ 17] = QColor(17, 17, 17);
  _colors[ 18] = QColor(18, 18, 18);
  _colors[ 19] = QColor(19, 19, 19);
  _colors[ 20] = QColor(20, 20, 20);
  _colors[ 21] = QColor(21, 21, 21);
  _colors[ 22] = QColor(22, 22, 22);
  _colors[ 23] = QColor(23, 23, 23);
  _colors[ 24] = QColor(24, 24, 24);
  _colors[ 25] = QColor(25, 25, 25);
  _colors[ 26] = QColor(26, 26, 26);
  _colors[ 27] = QColor(27, 27, 27);
  _colors[ 28] = QColor(28, 28, 28);
  _colors[ 29] = QColor(29, 29, 29);
  _colors[ 30] = QColor(30, 30, 30);
  _colors[ 31] = QColor(31, 31, 31);
  _colors[ 32] = QColor(32, 32, 32);
  _colors[ 33] = QColor(33, 33, 33);
  _colors[ 34] = QColor(34, 34, 34);
  _colors[ 35] = QColor(35, 35, 35);
  _colors[ 36] = QColor(36, 36, 36);
  _colors[ 37] = QColor(37, 37, 37);
  _colors[ 38] = QColor(38, 38, 38);
  _colors[ 39] = QColor(39, 39, 39);
  _colors[ 40] = QColor(40, 40, 40);
  _colors[ 41] = QColor(41, 41, 41);
  _colors[ 42] = QColor(42, 42, 42);
  _colors[ 43] = QColor(43, 43, 43);
  _colors[ 44] = QColor(44, 44, 44);
  _colors[ 45] = QColor(45, 45, 45);
  _colors[ 46] = QColor(46, 46, 46);
  _colors[ 47] = QColor(47, 47, 47);
  _colors[ 48] = QColor(48, 48, 48);
  _colors[ 49] = QColor(49, 49, 49);
  _colors[ 50] = QColor(50, 50, 50);
  _colors[ 51] = QColor(51, 51, 51);
  _colors[ 52] = QColor(52, 52, 52);
  _colors[ 53] = QColor(53, 53, 53);
  _colors[ 54] = QColor(54, 54, 54);
  _colors[ 55] = QColor(55, 55, 55);
  _colors[ 56] = QColor(56, 56, 56);
  _colors[ 57] = QColor(57, 57, 57);
  _colors[ 58] = QColor(58, 58, 58);
  _colors[ 59] = QColor(59, 59, 59);
  _colors[ 60] = QColor(60, 60, 60);
  _colors[ 61] = QColor(61, 61, 61);
  _colors[ 62] = QColor(62, 62, 62);
  _colors[ 63] = QColor(63, 63, 63);
  _colors[ 64] = QColor(64, 64, 64);
  _colors[ 65] = QColor(65, 65, 65);
  _colors[ 66] = QColor(66, 66, 66);
  _colors[ 67] = QColor(67, 67, 67);
  _colors[ 68] = QColor(68, 68, 68);
  _colors[ 69] = QColor(69, 69, 69);
  _colors[ 70] = QColor(70, 70, 70);
  _colors[ 71] = QColor(71, 71, 71);
  _colors[ 72] = QColor(72, 72, 72);
  _colors[ 73] = QColor(73, 73, 73);
  _colors[ 74] = QColor(74, 74, 74);
  _colors[ 75] = QColor(75, 75, 75);
  _colors[ 76] = QColor(76, 76, 76);
  _colors[ 77] = QColor(77, 77, 77);
  _colors[ 78] = QColor(78, 78, 78);
  _colors[ 79] = QColor(79, 79, 79);
  _colors[ 80] = QColor(80, 80, 80);
  _colors[ 81] = QColor(81, 81, 81);
  _colors[ 82] = QColor(82, 82, 82);
  _colors[ 83] = QColor(83, 83, 83);
  _colors[ 84] = QColor(84, 84, 84);
  _colors[ 85] = QColor(85, 85, 85);
  _colors[ 86] = QColor(86, 86, 86);
  _colors[ 87] = QColor(87, 87, 87);
  _colors[ 88] = QColor(88, 88, 88);
  _colors[ 89] = QColor(89, 89, 89);
  _colors[ 90] = QColor(90, 90, 90);
  _colors[ 91] = QColor(91, 91, 91);
  _colors[ 92] = QColor(92, 92, 92);
  _colors[ 93] = QColor(93, 93, 93);
  _colors[ 94] = QColor(94, 94, 94);
  _colors[ 95] = QColor(95, 95, 95);
  _colors[ 96] = QColor(96, 96, 96);
  _colors[ 97] = QColor(97, 97, 97);
  _colors[ 98] = QColor(98, 98, 98);
  _colors[ 99] = QColor(99, 99, 99);
  _colors[100] = QColor(100, 100, 100);
  _colors[101] = QColor(101, 101, 101);
  _colors[102] = QColor(102, 102, 102);
  _colors[103] = QColor(103, 103, 103);
  _colors[104] = QColor(104, 104, 104);
  _colors[105] = QColor(105, 105, 105);
  _colors[106] = QColor(106, 106, 106);
  _colors[107] = QColor(107, 107, 107);
  _colors[108] = QColor(108, 108, 108);
  _colors[109] = QColor(109, 109, 109);
  _colors[110] = QColor(110, 110, 110);
  _colors[111] = QColor(111, 111, 111);
  _colors[112] = QColor(112, 112, 112);
  _colors[113] = QColor(113, 113, 113);
  _colors[114] = QColor(114, 114, 114);
  _colors[115] = QColor(115, 115, 115);
  _colors[116] = QColor(116, 116, 116);
  _colors[117] = QColor(117, 117, 117);
  _colors[118] = QColor(118, 118, 118);
  _colors[119] = QColor(119, 119, 119);
  _colors[120] = QColor(120, 120, 120);
  _colors[121] = QColor(121, 121, 121);
  _colors[122] = QColor(122, 122, 122);
  _colors[123] = QColor(123, 123, 123);
  _colors[124] = QColor(124, 124, 124);
  _colors[125] = QColor(125, 125, 125);
  _colors[126] = QColor(126, 126, 126);
  _colors[127] = QColor(127, 127, 127);
  _colors[128] = QColor(128, 128, 128);
  _colors[129] = QColor(129, 129, 129);
  _colors[130] = QColor(130, 130, 130);
  _colors[131] = QColor(131, 131, 131);
  _colors[132] = QColor(132, 132, 132);
  _colors[133] = QColor(133, 133, 133);
  _colors[134] = QColor(134, 134, 134);
  _colors[135] = QColor(135, 135, 135);
  _colors[136] = QColor(136, 136, 136);
  _colors[137] = QColor(137, 137, 137);
  _colors[138] = QColor(138, 138, 138);
  _colors[139] = QColor(139, 139, 139);
  _colors[140] = QColor(140, 140, 140);
  _colors[141] = QColor(141, 141, 141);
  _colors[142] = QColor(142, 142, 142);
  _colors[143] = QColor(143, 143, 143);
  _colors[144] = QColor(144, 144, 144);
  _colors[145] = QColor(145, 145, 145);
  _colors[146] = QColor(146, 146, 146);
  _colors[147] = QColor(147, 147, 147);
  _colors[148] = QColor(148, 148, 148);
  _colors[149] = QColor(149, 149, 149);
  _colors[150] = QColor(150, 150, 150);
  _colors[151] = QColor(151, 151, 151);
  _colors[152] = QColor(152, 152, 152);
  _colors[153] = QColor(153, 153, 153);
  _colors[154] = QColor(154, 154, 154);
  _colors[155] = QColor(155, 155, 155);
  _colors[156] = QColor(156, 156, 156);
  _colors[157] = QColor(157, 157, 157);
  _colors[158] = QColor(158, 158, 158);
  _colors[159] = QColor(159, 159, 159);
  _colors[160] = QColor(160, 160, 160);
  _colors[161] = QColor(161, 161, 161);
  _colors[162] = QColor(162, 162, 162);
  _colors[163] = QColor(163, 163, 163);
  _colors[164] = QColor(164, 164, 164);
  _colors[165] = QColor(165, 165, 165);
  _colors[166] = QColor(166, 166, 166);
  _colors[167] = QColor(167, 167, 167);
  _colors[168] = QColor(168, 168, 168);
  _colors[169] = QColor(169, 169, 169);
  _colors[170] = QColor(170, 170, 170);
  _colors[171] = QColor(171, 171, 171);
  _colors[172] = QColor(172, 172, 172);
  _colors[173] = QColor(173, 173, 173);
  _colors[174] = QColor(174, 174, 174);
  _colors[175] = QColor(175, 175, 175);
  _colors[176] = QColor(176, 176, 176);
  _colors[177] = QColor(177, 177, 177);
  _colors[178] = QColor(178, 178, 178);
  _colors[179] = QColor(179, 179, 179);
  _colors[180] = QColor(180, 180, 180);
  _colors[181] = QColor(181, 181, 181);
  _colors[182] = QColor(182, 182, 182);
  _colors[183] = QColor(183, 183, 183);
  _colors[184] = QColor(184, 184, 184);
  _colors[185] = QColor(185, 185, 185);
  _colors[186] = QColor(186, 186, 186);
  _colors[187] = QColor(187, 187, 187);
  _colors[188] = QColor(188, 188, 188);
  _colors[189] = QColor(189, 189, 189);
  _colors[190] = QColor(190, 190, 190);
  _colors[191] = QColor(191, 191, 191);
  _colors[192] = QColor(192, 192, 192);
  _colors[193] = QColor(193, 193, 193);
  _colors[194] = QColor(194, 194, 194);
  _colors[195] = QColor(195, 195, 195);
  _colors[196] = QColor(196, 196, 196);
  _colors[197] = QColor(197, 197, 197);
  _colors[198] = QColor(198, 198, 198);
  _colors[199] = QColor(199, 199, 199);
  _colors[200] = QColor(200, 200, 200);
  _colors[201] = QColor(201, 201, 201);
  _colors[202] = QColor(202, 202, 202);
  _colors[203] = QColor(203, 203, 203);
  _colors[204] = QColor(204, 204, 204);
  _colors[205] = QColor(205, 205, 205);
  _colors[206] = QColor(206, 206, 206);
  _colors[207] = QColor(207, 207, 207);
  _colors[208] = QColor(208, 208, 208);
  _colors[209] = QColor(209, 209, 209);
  _colors[210] = QColor(210, 210, 210);
  _colors[211] = QColor(211, 211, 211);
  _colors[212] = QColor(212, 212, 212);
  _colors[213] = QColor(213, 213, 213);
  _colors[214] = QColor(214, 214, 214);
  _colors[215] = QColor(215, 215, 215);
  _colors[216] = QColor(216, 216, 216);
  _colors[217] = QColor(217, 217, 217);
  _colors[218] = QColor(218, 218, 218);
  _colors[219] = QColor(219, 219, 219);
  _colors[220] = QColor(220, 220, 220);
  _colors[221] = QColor(221, 221, 221);
  _colors[222] = QColor(222, 222, 222);
  _colors[223] = QColor(223, 223, 223);
  _colors[224] = QColor(224, 224, 224);
  _colors[225] = QColor(225, 225, 225);
  _colors[226] = QColor(226, 226, 226);
  _colors[227] = QColor(227, 227, 227);
  _colors[228] = QColor(228, 228, 228);
  _colors[229] = QColor(229, 229, 229);
  _colors[230] = QColor(230, 230, 230);
  _colors[231] = QColor(231, 231, 231);
  _colors[232] = QColor(232, 232, 232);
  _colors[233] = QColor(233, 233, 233);
  _colors[234] = QColor(234, 234, 234);
  _colors[235] = QColor(235, 235, 235);
  _colors[236] = QColor(236, 236, 236);
  _colors[237] = QColor(237, 237, 237);
  _colors[238] = QColor(238, 238, 238);
  _colors[239] = QColor(239, 239, 239);
  _colors[240] = QColor(240, 240, 240);
  _colors[241] = QColor(241, 241, 241);
  _colors[242] = QColor(242, 242, 242);
  _colors[243] = QColor(243, 243, 243);
  _colors[244] = QColor(244, 244, 244);
  _colors[245] = QColor(245, 245, 245);
  _colors[246] = QColor(246, 246, 246);
  _colors[247] = QColor(247, 247, 247);
  _colors[248] = QColor(248, 248, 248);
  _colors[249] = QColor(249, 249, 249);
  _colors[250] = QColor(250, 250, 250);
  _colors[251] = QColor(251, 251, 251);
  _colors[252] = QColor(252, 252, 252);
  _colors[253] = QColor(253, 253, 253);
  _colors[254] = QColor(254, 254, 254);
  _colors[255] = QColor(255, 255, 255);

}
