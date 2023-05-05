#include "test.h"
#include <iostream>
#include <list>

struct TileEntity
{
	float x;
	float y;
	float z;
};


class Test3 : public Test
{
public:
	Test3()
	{
		float halfSizeX = 0.5;float halfSizeY = 0.5;std::list<TileEntity> tiles;TileEntity t0;t0.x=-14;t0.y=-9;t0.z=0;tiles.push_back(t0);TileEntity t1;t1.x=-14;t1.y=-8;t1.z=0;tiles.push_back(t1);TileEntity t2;t2.x=-14;t2.y=-7;t2.z=0;tiles.push_back(t2);TileEntity t3;t3.x=-14;t3.y=-6;t3.z=0;tiles.push_back(t3);TileEntity t4;t4.x=-14;t4.y=-5;t4.z=0;tiles.push_back(t4);TileEntity t5;t5.x=-14;t5.y=-4;t5.z=0;tiles.push_back(t5);TileEntity t6;t6.x=-14;t6.y=-3;t6.z=0;tiles.push_back(t6);TileEntity t7;t7.x=-14;t7.y=-2;t7.z=0;tiles.push_back(t7);TileEntity t8;t8.x=-14;t8.y=-1;t8.z=0;tiles.push_back(t8);TileEntity t9;t9.x=-14;t9.y=0;t9.z=0;tiles.push_back(t9);TileEntity t10;t10.x=-14;t10.y=1;t10.z=0;tiles.push_back(t10);TileEntity t11;t11.x=-14;t11.y=2;t11.z=0;tiles.push_back(t11);TileEntity t12;t12.x=-14;t12.y=3;t12.z=0;tiles.push_back(t12);TileEntity t13;t13.x=-14;t13.y=4;t13.z=0;tiles.push_back(t13);TileEntity t14;t14.x=-14;t14.y=5;t14.z=0;tiles.push_back(t14);TileEntity t15;t15.x=-14;t15.y=6;t15.z=0;tiles.push_back(t15);TileEntity t16;t16.x=-14;t16.y=7;t16.z=0;tiles.push_back(t16);TileEntity t17;t17.x=-14;t17.y=8;t17.z=0;tiles.push_back(t17);TileEntity t18;t18.x=-14;t18.y=9;t18.z=0;tiles.push_back(t18);TileEntity t19;t19.x=-14;t19.y=10;t19.z=0;tiles.push_back(t19);TileEntity t20;t20.x=-14;t20.y=11;t20.z=0;tiles.push_back(t20);TileEntity t21;t21.x=-14;t21.y=12;t21.z=0;tiles.push_back(t21);TileEntity t22;t22.x=-14;t22.y=13;t22.z=0;tiles.push_back(t22);TileEntity t23;t23.x=-14;t23.y=14;t23.z=0;tiles.push_back(t23);TileEntity t24;t24.x=-14;t24.y=15;t24.z=0;tiles.push_back(t24);TileEntity t25;t25.x=-14;t25.y=16;t25.z=0;tiles.push_back(t25);TileEntity t26;t26.x=-14;t26.y=17;t26.z=0;tiles.push_back(t26);TileEntity t27;t27.x=-14;t27.y=18;t27.z=0;tiles.push_back(t27);TileEntity t28;t28.x=-14;t28.y=19;t28.z=0;tiles.push_back(t28);TileEntity t29;t29.x=-13;t29.y=-9;t29.z=0;tiles.push_back(t29);TileEntity t30;t30.x=-13;t30.y=-6;t30.z=0;tiles.push_back(t30);TileEntity t31;t31.x=-13;t31.y=5;t31.z=0;tiles.push_back(t31);TileEntity t32;t32.x=-13;t32.y=6;t32.z=0;tiles.push_back(t32);TileEntity t33;t33.x=-13;t33.y=7;t33.z=0;tiles.push_back(t33);TileEntity t34;t34.x=-13;t34.y=8;t34.z=0;tiles.push_back(t34);TileEntity t35;t35.x=-13;t35.y=9;t35.z=0;tiles.push_back(t35);TileEntity t36;t36.x=-13;t36.y=10;t36.z=0;tiles.push_back(t36);TileEntity t37;t37.x=-13;t37.y=11;t37.z=0;tiles.push_back(t37);TileEntity t38;t38.x=-13;t38.y=19;t38.z=0;tiles.push_back(t38);TileEntity t39;t39.x=-12;t39.y=-9;t39.z=0;tiles.push_back(t39);TileEntity t40;t40.x=-12;t40.y=-6;t40.z=0;tiles.push_back(t40);TileEntity t41;t41.x=-12;t41.y=3;t41.z=0;tiles.push_back(t41);TileEntity t42;t42.x=-12;t42.y=6;t42.z=0;tiles.push_back(t42);TileEntity t43;t43.x=-12;t43.y=7;t43.z=0;tiles.push_back(t43);TileEntity t44;t44.x=-12;t44.y=8;t44.z=0;tiles.push_back(t44);TileEntity t45;t45.x=-12;t45.y=9;t45.z=0;tiles.push_back(t45);TileEntity t46;t46.x=-12;t46.y=10;t46.z=0;tiles.push_back(t46);TileEntity t47;t47.x=-12;t47.y=11;t47.z=0;tiles.push_back(t47);TileEntity t48;t48.x=-12;t48.y=19;t48.z=0;tiles.push_back(t48);TileEntity t49;t49.x=-11;t49.y=-9;t49.z=0;tiles.push_back(t49);TileEntity t50;t50.x=-11;t50.y=-6;t50.z=0;tiles.push_back(t50);TileEntity t51;t51.x=-11;t51.y=3;t51.z=0;tiles.push_back(t51);TileEntity t52;t52.x=-11;t52.y=6;t52.z=0;tiles.push_back(t52);TileEntity t53;t53.x=-11;t53.y=7;t53.z=0;tiles.push_back(t53);TileEntity t54;t54.x=-11;t54.y=8;t54.z=0;tiles.push_back(t54);TileEntity t55;t55.x=-11;t55.y=9;t55.z=0;tiles.push_back(t55);TileEntity t56;t56.x=-11;t56.y=10;t56.z=0;tiles.push_back(t56);TileEntity t57;t57.x=-11;t57.y=11;t57.z=0;tiles.push_back(t57);TileEntity t58;t58.x=-11;t58.y=19;t58.z=0;tiles.push_back(t58);TileEntity t59;t59.x=-10;t59.y=-9;t59.z=0;tiles.push_back(t59);TileEntity t60;t60.x=-10;t60.y=-6;t60.z=0;tiles.push_back(t60);TileEntity t61;t61.x=-10;t61.y=-5;t61.z=0;tiles.push_back(t61);TileEntity t62;t62.x=-10;t62.y=-3;t62.z=0;tiles.push_back(t62);TileEntity t63;t63.x=-10;t63.y=-2;t63.z=0;tiles.push_back(t63);TileEntity t64;t64.x=-10;t64.y=-1;t64.z=0;tiles.push_back(t64);TileEntity t65;t65.x=-10;t65.y=3;t65.z=0;tiles.push_back(t65);TileEntity t66;t66.x=-10;t66.y=6;t66.z=0;tiles.push_back(t66);TileEntity t67;t67.x=-10;t67.y=7;t67.z=0;tiles.push_back(t67);TileEntity t68;t68.x=-10;t68.y=8;t68.z=0;tiles.push_back(t68);TileEntity t69;t69.x=-10;t69.y=9;t69.z=0;tiles.push_back(t69);TileEntity t70;t70.x=-10;t70.y=10;t70.z=0;tiles.push_back(t70);TileEntity t71;t71.x=-10;t71.y=11;t71.z=0;tiles.push_back(t71);TileEntity t72;t72.x=-10;t72.y=19;t72.z=0;tiles.push_back(t72);TileEntity t73;t73.x=-9;t73.y=-9;t73.z=0;tiles.push_back(t73);TileEntity t74;t74.x=-9;t74.y=-3;t74.z=0;tiles.push_back(t74);TileEntity t75;t75.x=-9;t75.y=-2;t75.z=0;tiles.push_back(t75);TileEntity t76;t76.x=-9;t76.y=-1;t76.z=0;tiles.push_back(t76);TileEntity t77;t77.x=-9;t77.y=3;t77.z=0;tiles.push_back(t77);TileEntity t78;t78.x=-9;t78.y=5;t78.z=0;tiles.push_back(t78);TileEntity t79;t79.x=-9;t79.y=6;t79.z=0;tiles.push_back(t79);TileEntity t80;t80.x=-9;t80.y=7;t80.z=0;tiles.push_back(t80);TileEntity t81;t81.x=-9;t81.y=8;t81.z=0;tiles.push_back(t81);TileEntity t82;t82.x=-9;t82.y=9;t82.z=0;tiles.push_back(t82);TileEntity t83;t83.x=-9;t83.y=10;t83.z=0;tiles.push_back(t83);TileEntity t84;t84.x=-9;t84.y=11;t84.z=0;tiles.push_back(t84);TileEntity t85;t85.x=-9;t85.y=19;t85.z=0;tiles.push_back(t85);TileEntity t86;t86.x=-8;t86.y=-9;t86.z=0;tiles.push_back(t86);TileEntity t87;t87.x=-8;t87.y=-3;t87.z=0;tiles.push_back(t87);TileEntity t88;t88.x=-8;t88.y=-2;t88.z=0;tiles.push_back(t88);TileEntity t89;t89.x=-8;t89.y=-1;t89.z=0;tiles.push_back(t89);TileEntity t90;t90.x=-8;t90.y=0;t90.z=0;tiles.push_back(t90);TileEntity t91;t91.x=-8;t91.y=1;t91.z=0;tiles.push_back(t91);TileEntity t92;t92.x=-8;t92.y=3;t92.z=0;tiles.push_back(t92);TileEntity t93;t93.x=-8;t93.y=5;t93.z=0;tiles.push_back(t93);TileEntity t94;t94.x=-8;t94.y=6;t94.z=0;tiles.push_back(t94);TileEntity t95;t95.x=-8;t95.y=7;t95.z=0;tiles.push_back(t95);TileEntity t96;t96.x=-8;t96.y=8;t96.z=0;tiles.push_back(t96);TileEntity t97;t97.x=-8;t97.y=9;t97.z=0;tiles.push_back(t97);TileEntity t98;t98.x=-8;t98.y=10;t98.z=0;tiles.push_back(t98);TileEntity t99;t99.x=-8;t99.y=11;t99.z=0;tiles.push_back(t99);TileEntity t100;t100.x=-8;t100.y=12;t100.z=0;tiles.push_back(t100);TileEntity t101;t101.x=-8;t101.y=13;t101.z=0;tiles.push_back(t101);TileEntity t102;t102.x=-8;t102.y=14;t102.z=0;tiles.push_back(t102);TileEntity t103;t103.x=-8;t103.y=15;t103.z=0;tiles.push_back(t103);TileEntity t104;t104.x=-8;t104.y=16;t104.z=0;tiles.push_back(t104);TileEntity t105;t105.x=-8;t105.y=19;t105.z=0;tiles.push_back(t105);TileEntity t106;t106.x=-7;t106.y=-9;t106.z=0;tiles.push_back(t106);TileEntity t107;t107.x=-7;t107.y=-5;t107.z=0;tiles.push_back(t107);TileEntity t108;t108.x=-7;t108.y=-2;t108.z=0;tiles.push_back(t108);TileEntity t109;t109.x=-7;t109.y=-1;t109.z=0;tiles.push_back(t109);TileEntity t110;t110.x=-7;t110.y=0;t110.z=0;tiles.push_back(t110);TileEntity t111;t111.x=-7;t111.y=1;t111.z=0;tiles.push_back(t111);TileEntity t112;t112.x=-7;t112.y=3;t112.z=0;tiles.push_back(t112);TileEntity t113;t113.x=-7;t113.y=7;t113.z=0;tiles.push_back(t113);TileEntity t114;t114.x=-7;t114.y=8;t114.z=0;tiles.push_back(t114);TileEntity t115;t115.x=-7;t115.y=9;t115.z=0;tiles.push_back(t115);TileEntity t116;t116.x=-7;t116.y=10;t116.z=0;tiles.push_back(t116);TileEntity t117;t117.x=-7;t117.y=11;t117.z=0;tiles.push_back(t117);TileEntity t118;t118.x=-7;t118.y=12;t118.z=0;tiles.push_back(t118);TileEntity t119;t119.x=-7;t119.y=13;t119.z=0;tiles.push_back(t119);TileEntity t120;t120.x=-7;t120.y=14;t120.z=0;tiles.push_back(t120);TileEntity t121;t121.x=-7;t121.y=15;t121.z=0;tiles.push_back(t121);TileEntity t122;t122.x=-7;t122.y=16;t122.z=0;tiles.push_back(t122);TileEntity t123;t123.x=-7;t123.y=19;t123.z=0;tiles.push_back(t123);TileEntity t124;t124.x=-6;t124.y=-9;t124.z=0;tiles.push_back(t124);TileEntity t125;t125.x=-6;t125.y=-5;t125.z=0;tiles.push_back(t125);TileEntity t126;t126.x=-6;t126.y=-4;t126.z=0;tiles.push_back(t126);TileEntity t127;t127.x=-6;t127.y=-2;t127.z=0;tiles.push_back(t127);TileEntity t128;t128.x=-6;t128.y=-1;t128.z=0;tiles.push_back(t128);TileEntity t129;t129.x=-6;t129.y=0;t129.z=0;tiles.push_back(t129);TileEntity t130;t130.x=-6;t130.y=1;t130.z=0;tiles.push_back(t130);TileEntity t131;t131.x=-6;t131.y=3;t131.z=0;tiles.push_back(t131);TileEntity t132;t132.x=-6;t132.y=7;t132.z=0;tiles.push_back(t132);TileEntity t133;t133.x=-6;t133.y=14;t133.z=0;tiles.push_back(t133);TileEntity t134;t134.x=-6;t134.y=16;t134.z=0;tiles.push_back(t134);TileEntity t135;t135.x=-6;t135.y=19;t135.z=0;tiles.push_back(t135);TileEntity t136;t136.x=-5;t136.y=-9;t136.z=0;tiles.push_back(t136);TileEntity t137;t137.x=-5;t137.y=-5;t137.z=0;tiles.push_back(t137);TileEntity t138;t138.x=-5;t138.y=-4;t138.z=0;tiles.push_back(t138);TileEntity t139;t139.x=-5;t139.y=-2;t139.z=0;tiles.push_back(t139);TileEntity t140;t140.x=-5;t140.y=-1;t140.z=0;tiles.push_back(t140);TileEntity t141;t141.x=-5;t141.y=0;t141.z=0;tiles.push_back(t141);TileEntity t142;t142.x=-5;t142.y=1;t142.z=0;tiles.push_back(t142);TileEntity t143;t143.x=-5;t143.y=6;t143.z=0;tiles.push_back(t143);TileEntity t144;t144.x=-5;t144.y=7;t144.z=0;tiles.push_back(t144);TileEntity t145;t145.x=-5;t145.y=10;t145.z=0;tiles.push_back(t145);TileEntity t146;t146.x=-5;t146.y=13;t146.z=0;tiles.push_back(t146);TileEntity t147;t147.x=-5;t147.y=14;t147.z=0;tiles.push_back(t147);TileEntity t148;t148.x=-5;t148.y=15;t148.z=0;tiles.push_back(t148);TileEntity t149;t149.x=-5;t149.y=16;t149.z=0;tiles.push_back(t149);TileEntity t150;t150.x=-5;t150.y=17;t150.z=0;tiles.push_back(t150);TileEntity t151;t151.x=-5;t151.y=19;t151.z=0;tiles.push_back(t151);TileEntity t152;t152.x=-4;t152.y=-9;t152.z=0;tiles.push_back(t152);TileEntity t153;t153.x=-4;t153.y=-5;t153.z=0;tiles.push_back(t153);TileEntity t154;t154.x=-4;t154.y=-4;t154.z=0;tiles.push_back(t154);TileEntity t155;t155.x=-4;t155.y=-2;t155.z=0;tiles.push_back(t155);TileEntity t156;t156.x=-4;t156.y=-1;t156.z=0;tiles.push_back(t156);TileEntity t157;t157.x=-4;t157.y=0;t157.z=0;tiles.push_back(t157);TileEntity t158;t158.x=-4;t158.y=6;t158.z=0;tiles.push_back(t158);TileEntity t159;t159.x=-4;t159.y=10;t159.z=0;tiles.push_back(t159);TileEntity t160;t160.x=-4;t160.y=13;t160.z=0;tiles.push_back(t160);TileEntity t161;t161.x=-4;t161.y=15;t161.z=0;tiles.push_back(t161);TileEntity t162;t162.x=-4;t162.y=16;t162.z=0;tiles.push_back(t162);TileEntity t163;t163.x=-4;t163.y=17;t163.z=0;tiles.push_back(t163);TileEntity t164;t164.x=-4;t164.y=19;t164.z=0;tiles.push_back(t164);TileEntity t165;t165.x=-3;t165.y=-9;t165.z=0;tiles.push_back(t165);TileEntity t166;t166.x=-3;t166.y=-5;t166.z=0;tiles.push_back(t166);TileEntity t167;t167.x=-3;t167.y=-4;t167.z=0;tiles.push_back(t167);TileEntity t168;t168.x=-3;t168.y=-2;t168.z=0;tiles.push_back(t168);TileEntity t169;t169.x=-3;t169.y=-1;t169.z=0;tiles.push_back(t169);TileEntity t170;t170.x=-3;t170.y=0;t170.z=0;tiles.push_back(t170);TileEntity t171;t171.x=-3;t171.y=6;t171.z=0;tiles.push_back(t171);TileEntity t172;t172.x=-3;t172.y=10;t172.z=0;tiles.push_back(t172);TileEntity t173;t173.x=-3;t173.y=13;t173.z=0;tiles.push_back(t173);TileEntity t174;t174.x=-3;t174.y=15;t174.z=0;tiles.push_back(t174);TileEntity t175;t175.x=-3;t175.y=16;t175.z=0;tiles.push_back(t175);TileEntity t176;t176.x=-3;t176.y=19;t176.z=0;tiles.push_back(t176);TileEntity t177;t177.x=-2;t177.y=-9;t177.z=0;tiles.push_back(t177);TileEntity t178;t178.x=-2;t178.y=-5;t178.z=0;tiles.push_back(t178);TileEntity t179;t179.x=-2;t179.y=-4;t179.z=0;tiles.push_back(t179);TileEntity t180;t180.x=-2;t180.y=-2;t180.z=0;tiles.push_back(t180);TileEntity t181;t181.x=-2;t181.y=-1;t181.z=0;tiles.push_back(t181);TileEntity t182;t182.x=-2;t182.y=0;t182.z=0;tiles.push_back(t182);TileEntity t183;t183.x=-2;t183.y=5;t183.z=0;tiles.push_back(t183);TileEntity t184;t184.x=-2;t184.y=6;t184.z=0;tiles.push_back(t184);TileEntity t185;t185.x=-2;t185.y=13;t185.z=0;tiles.push_back(t185);TileEntity t186;t186.x=-2;t186.y=15;t186.z=0;tiles.push_back(t186);TileEntity t187;t187.x=-2;t187.y=16;t187.z=0;tiles.push_back(t187);TileEntity t188;t188.x=-2;t188.y=19;t188.z=0;tiles.push_back(t188);TileEntity t189;t189.x=-1;t189.y=-9;t189.z=0;tiles.push_back(t189);TileEntity t190;t190.x=-1;t190.y=-5;t190.z=0;tiles.push_back(t190);TileEntity t191;t191.x=-1;t191.y=-4;t191.z=0;tiles.push_back(t191);TileEntity t192;t192.x=-1;t192.y=-2;t192.z=0;tiles.push_back(t192);TileEntity t193;t193.x=-1;t193.y=-1;t193.z=0;tiles.push_back(t193);TileEntity t194;t194.x=-1;t194.y=0;t194.z=0;tiles.push_back(t194);TileEntity t195;t195.x=-1;t195.y=2;t195.z=0;tiles.push_back(t195);TileEntity t196;t196.x=-1;t196.y=3;t196.z=0;tiles.push_back(t196);TileEntity t197;t197.x=-1;t197.y=4;t197.z=0;tiles.push_back(t197);TileEntity t198;t198.x=-1;t198.y=5;t198.z=0;tiles.push_back(t198);TileEntity t199;t199.x=-1;t199.y=6;t199.z=0;tiles.push_back(t199);TileEntity t200;t200.x=-1;t200.y=12;t200.z=0;tiles.push_back(t200);TileEntity t201;t201.x=-1;t201.y=13;t201.z=0;tiles.push_back(t201);TileEntity t202;t202.x=-1;t202.y=19;t202.z=0;tiles.push_back(t202);TileEntity t203;t203.x=0;t203.y=-9;t203.z=0;tiles.push_back(t203);TileEntity t204;t204.x=0;t204.y=-5;t204.z=0;tiles.push_back(t204);TileEntity t205;t205.x=0;t205.y=-4;t205.z=0;tiles.push_back(t205);TileEntity t206;t206.x=0;t206.y=-2;t206.z=0;tiles.push_back(t206);TileEntity t207;t207.x=0;t207.y=-1;t207.z=0;tiles.push_back(t207);TileEntity t208;t208.x=0;t208.y=0;t208.z=0;tiles.push_back(t208);TileEntity t209;t209.x=0;t209.y=1;t209.z=0;tiles.push_back(t209);TileEntity t210;t210.x=0;t210.y=2;t210.z=0;tiles.push_back(t210);TileEntity t211;t211.x=0;t211.y=5;t211.z=0;tiles.push_back(t211);TileEntity t212;t212.x=0;t212.y=6;t212.z=0;tiles.push_back(t212);TileEntity t213;t213.x=0;t213.y=9;t213.z=0;tiles.push_back(t213);TileEntity t214;t214.x=0;t214.y=10;t214.z=0;tiles.push_back(t214);TileEntity t215;t215.x=0;t215.y=12;t215.z=0;tiles.push_back(t215);TileEntity t216;t216.x=0;t216.y=19;t216.z=0;tiles.push_back(t216);TileEntity t217;t217.x=1;t217.y=-9;t217.z=0;tiles.push_back(t217);TileEntity t218;t218.x=1;t218.y=-8;t218.z=0;tiles.push_back(t218);TileEntity t219;t219.x=1;t219.y=-7;t219.z=0;tiles.push_back(t219);TileEntity t220;t220.x=1;t220.y=-4;t220.z=0;tiles.push_back(t220);TileEntity t221;t221.x=1;t221.y=-2;t221.z=0;tiles.push_back(t221);TileEntity t222;t222.x=1;t222.y=-1;t222.z=0;tiles.push_back(t222);TileEntity t223;t223.x=1;t223.y=0;t223.z=0;tiles.push_back(t223);TileEntity t224;t224.x=1;t224.y=5;t224.z=0;tiles.push_back(t224);TileEntity t225;t225.x=1;t225.y=6;t225.z=0;tiles.push_back(t225);TileEntity t226;t226.x=1;t226.y=9;t226.z=0;tiles.push_back(t226);TileEntity t227;t227.x=1;t227.y=12;t227.z=0;tiles.push_back(t227);TileEntity t228;t228.x=1;t228.y=14;t228.z=0;tiles.push_back(t228);TileEntity t229;t229.x=1;t229.y=19;t229.z=0;tiles.push_back(t229);TileEntity t230;t230.x=2;t230.y=-9;t230.z=0;tiles.push_back(t230);TileEntity t231;t231.x=2;t231.y=-8;t231.z=0;tiles.push_back(t231);TileEntity t232;t232.x=2;t232.y=-7;t232.z=0;tiles.push_back(t232);TileEntity t233;t233.x=2;t233.y=-2;t233.z=0;tiles.push_back(t233);TileEntity t234;t234.x=2;t234.y=-1;t234.z=0;tiles.push_back(t234);TileEntity t235;t235.x=2;t235.y=0;t235.z=0;tiles.push_back(t235);TileEntity t236;t236.x=2;t236.y=4;t236.z=0;tiles.push_back(t236);TileEntity t237;t237.x=2;t237.y=5;t237.z=0;tiles.push_back(t237);TileEntity t238;t238.x=2;t238.y=6;t238.z=0;tiles.push_back(t238);TileEntity t239;t239.x=2;t239.y=9;t239.z=0;tiles.push_back(t239);TileEntity t240;t240.x=2;t240.y=12;t240.z=0;tiles.push_back(t240);TileEntity t241;t241.x=2;t241.y=13;t241.z=0;tiles.push_back(t241);TileEntity t242;t242.x=2;t242.y=14;t242.z=0;tiles.push_back(t242);TileEntity t243;t243.x=2;t243.y=19;t243.z=0;tiles.push_back(t243);TileEntity t244;t244.x=3;t244.y=-9;t244.z=0;tiles.push_back(t244);TileEntity t245;t245.x=3;t245.y=-8;t245.z=0;tiles.push_back(t245);TileEntity t246;t246.x=3;t246.y=-7;t246.z=0;tiles.push_back(t246);TileEntity t247;t247.x=3;t247.y=-2;t247.z=0;tiles.push_back(t247);TileEntity t248;t248.x=3;t248.y=-1;t248.z=0;tiles.push_back(t248);TileEntity t249;t249.x=3;t249.y=0;t249.z=0;tiles.push_back(t249);TileEntity t250;t250.x=3;t250.y=2;t250.z=0;tiles.push_back(t250);TileEntity t251;t251.x=3;t251.y=3;t251.z=0;tiles.push_back(t251);TileEntity t252;t252.x=3;t252.y=5;t252.z=0;tiles.push_back(t252);TileEntity t253;t253.x=3;t253.y=6;t253.z=0;tiles.push_back(t253);TileEntity t254;t254.x=3;t254.y=9;t254.z=0;tiles.push_back(t254);TileEntity t255;t255.x=3;t255.y=12;t255.z=0;tiles.push_back(t255);TileEntity t256;t256.x=3;t256.y=13;t256.z=0;tiles.push_back(t256);TileEntity t257;t257.x=3;t257.y=19;t257.z=0;tiles.push_back(t257);TileEntity t258;t258.x=4;t258.y=-9;t258.z=0;tiles.push_back(t258);TileEntity t259;t259.x=4;t259.y=-2;t259.z=0;tiles.push_back(t259);TileEntity t260;t260.x=4;t260.y=-1;t260.z=0;tiles.push_back(t260);TileEntity t261;t261.x=4;t261.y=0;t261.z=0;tiles.push_back(t261);TileEntity t262;t262.x=4;t262.y=1;t262.z=0;tiles.push_back(t262);TileEntity t263;t263.x=4;t263.y=5;t263.z=0;tiles.push_back(t263);TileEntity t264;t264.x=4;t264.y=6;t264.z=0;tiles.push_back(t264);TileEntity t265;t265.x=4;t265.y=8;t265.z=0;tiles.push_back(t265);TileEntity t266;t266.x=4;t266.y=9;t266.z=0;tiles.push_back(t266);TileEntity t267;t267.x=4;t267.y=12;t267.z=0;tiles.push_back(t267);TileEntity t268;t268.x=4;t268.y=13;t268.z=0;tiles.push_back(t268);TileEntity t269;t269.x=4;t269.y=19;t269.z=0;tiles.push_back(t269);TileEntity t270;t270.x=5;t270.y=-9;t270.z=0;tiles.push_back(t270);TileEntity t271;t271.x=5;t271.y=-1;t271.z=0;tiles.push_back(t271);TileEntity t272;t272.x=5;t272.y=0;t272.z=0;tiles.push_back(t272);TileEntity t273;t273.x=5;t273.y=1;t273.z=0;tiles.push_back(t273);TileEntity t274;t274.x=5;t274.y=3;t274.z=0;tiles.push_back(t274);TileEntity t275;t275.x=5;t275.y=5;t275.z=0;tiles.push_back(t275);TileEntity t276;t276.x=5;t276.y=6;t276.z=0;tiles.push_back(t276);TileEntity t277;t277.x=5;t277.y=12;t277.z=0;tiles.push_back(t277);TileEntity t278;t278.x=5;t278.y=13;t278.z=0;tiles.push_back(t278);TileEntity t279;t279.x=5;t279.y=19;t279.z=0;tiles.push_back(t279);TileEntity t280;t280.x=6;t280.y=-9;t280.z=0;tiles.push_back(t280);TileEntity t281;t281.x=6;t281.y=-1;t281.z=0;tiles.push_back(t281);TileEntity t282;t282.x=6;t282.y=0;t282.z=0;tiles.push_back(t282);TileEntity t283;t283.x=6;t283.y=1;t283.z=0;tiles.push_back(t283);TileEntity t284;t284.x=6;t284.y=3;t284.z=0;tiles.push_back(t284);TileEntity t285;t285.x=6;t285.y=6;t285.z=0;tiles.push_back(t285);TileEntity t286;t286.x=6;t286.y=12;t286.z=0;tiles.push_back(t286);TileEntity t287;t287.x=6;t287.y=13;t287.z=0;tiles.push_back(t287);TileEntity t288;t288.x=6;t288.y=19;t288.z=0;tiles.push_back(t288);TileEntity t289;t289.x=7;t289.y=-9;t289.z=0;tiles.push_back(t289);TileEntity t290;t290.x=7;t290.y=-1;t290.z=0;tiles.push_back(t290);TileEntity t291;t291.x=7;t291.y=0;t291.z=0;tiles.push_back(t291);TileEntity t292;t292.x=7;t292.y=1;t292.z=0;tiles.push_back(t292);TileEntity t293;t293.x=7;t293.y=3;t293.z=0;tiles.push_back(t293);TileEntity t294;t294.x=7;t294.y=6;t294.z=0;tiles.push_back(t294);TileEntity t295;t295.x=7;t295.y=7;t295.z=0;tiles.push_back(t295);TileEntity t296;t296.x=7;t296.y=12;t296.z=0;tiles.push_back(t296);TileEntity t297;t297.x=7;t297.y=13;t297.z=0;tiles.push_back(t297);TileEntity t298;t298.x=7;t298.y=17;t298.z=0;tiles.push_back(t298);TileEntity t299;t299.x=7;t299.y=19;t299.z=0;tiles.push_back(t299);TileEntity t300;t300.x=8;t300.y=-9;t300.z=0;tiles.push_back(t300);TileEntity t301;t301.x=8;t301.y=-1;t301.z=0;tiles.push_back(t301);TileEntity t302;t302.x=8;t302.y=0;t302.z=0;tiles.push_back(t302);TileEntity t303;t303.x=8;t303.y=1;t303.z=0;tiles.push_back(t303);TileEntity t304;t304.x=8;t304.y=3;t304.z=0;tiles.push_back(t304);TileEntity t305;t305.x=8;t305.y=7;t305.z=0;tiles.push_back(t305);TileEntity t306;t306.x=8;t306.y=10;t306.z=0;tiles.push_back(t306);TileEntity t307;t307.x=8;t307.y=12;t307.z=0;tiles.push_back(t307);TileEntity t308;t308.x=8;t308.y=13;t308.z=0;tiles.push_back(t308);TileEntity t309;t309.x=8;t309.y=16;t309.z=0;tiles.push_back(t309);TileEntity t310;t310.x=8;t310.y=17;t310.z=0;tiles.push_back(t310);TileEntity t311;t311.x=8;t311.y=19;t311.z=0;tiles.push_back(t311);TileEntity t312;t312.x=9;t312.y=-9;t312.z=0;tiles.push_back(t312);TileEntity t313;t313.x=9;t313.y=-4;t313.z=0;tiles.push_back(t313);TileEntity t314;t314.x=9;t314.y=-3;t314.z=0;tiles.push_back(t314);TileEntity t315;t315.x=9;t315.y=-2;t315.z=0;tiles.push_back(t315);TileEntity t316;t316.x=9;t316.y=-1;t316.z=0;tiles.push_back(t316);TileEntity t317;t317.x=9;t317.y=0;t317.z=0;tiles.push_back(t317);TileEntity t318;t318.x=9;t318.y=1;t318.z=0;tiles.push_back(t318);TileEntity t319;t319.x=9;t319.y=3;t319.z=0;tiles.push_back(t319);TileEntity t320;t320.x=9;t320.y=4;t320.z=0;tiles.push_back(t320);TileEntity t321;t321.x=9;t321.y=7;t321.z=0;tiles.push_back(t321);TileEntity t322;t322.x=9;t322.y=10;t322.z=0;tiles.push_back(t322);TileEntity t323;t323.x=9;t323.y=12;t323.z=0;tiles.push_back(t323);TileEntity t324;t324.x=9;t324.y=13;t324.z=0;tiles.push_back(t324);TileEntity t325;t325.x=9;t325.y=16;t325.z=0;tiles.push_back(t325);TileEntity t326;t326.x=9;t326.y=17;t326.z=0;tiles.push_back(t326);TileEntity t327;t327.x=9;t327.y=19;t327.z=0;tiles.push_back(t327);TileEntity t328;t328.x=10;t328.y=-9;t328.z=0;tiles.push_back(t328);TileEntity t329;t329.x=10;t329.y=-4;t329.z=0;tiles.push_back(t329);TileEntity t330;t330.x=10;t330.y=-1;t330.z=0;tiles.push_back(t330);TileEntity t331;t331.x=10;t331.y=0;t331.z=0;tiles.push_back(t331);TileEntity t332;t332.x=10;t332.y=1;t332.z=0;tiles.push_back(t332);TileEntity t333;t333.x=10;t333.y=2;t333.z=0;tiles.push_back(t333);TileEntity t334;t334.x=10;t334.y=4;t334.z=0;tiles.push_back(t334);TileEntity t335;t335.x=10;t335.y=7;t335.z=0;tiles.push_back(t335);TileEntity t336;t336.x=10;t336.y=8;t336.z=0;tiles.push_back(t336);TileEntity t337;t337.x=10;t337.y=10;t337.z=0;tiles.push_back(t337);TileEntity t338;t338.x=10;t338.y=12;t338.z=0;tiles.push_back(t338);TileEntity t339;t339.x=10;t339.y=13;t339.z=0;tiles.push_back(t339);TileEntity t340;t340.x=10;t340.y=16;t340.z=0;tiles.push_back(t340);TileEntity t341;t341.x=10;t341.y=17;t341.z=0;tiles.push_back(t341);TileEntity t342;t342.x=10;t342.y=19;t342.z=0;tiles.push_back(t342);TileEntity t343;t343.x=11;t343.y=-9;t343.z=0;tiles.push_back(t343);TileEntity t344;t344.x=11;t344.y=-4;t344.z=0;tiles.push_back(t344);TileEntity t345;t345.x=11;t345.y=-1;t345.z=0;tiles.push_back(t345);TileEntity t346;t346.x=11;t346.y=1;t346.z=0;tiles.push_back(t346);TileEntity t347;t347.x=11;t347.y=2;t347.z=0;tiles.push_back(t347);TileEntity t348;t348.x=11;t348.y=4;t348.z=0;tiles.push_back(t348);TileEntity t349;t349.x=11;t349.y=8;t349.z=0;tiles.push_back(t349);TileEntity t350;t350.x=11;t350.y=10;t350.z=0;tiles.push_back(t350);TileEntity t351;t351.x=11;t351.y=12;t351.z=0;tiles.push_back(t351);TileEntity t352;t352.x=11;t352.y=13;t352.z=0;tiles.push_back(t352);TileEntity t353;t353.x=11;t353.y=16;t353.z=0;tiles.push_back(t353);TileEntity t354;t354.x=11;t354.y=17;t354.z=0;tiles.push_back(t354);TileEntity t355;t355.x=11;t355.y=19;t355.z=0;tiles.push_back(t355);TileEntity t356;t356.x=12;t356.y=-9;t356.z=0;tiles.push_back(t356);TileEntity t357;t357.x=12;t357.y=-5;t357.z=0;tiles.push_back(t357);TileEntity t358;t358.x=12;t358.y=-4;t358.z=0;tiles.push_back(t358);TileEntity t359;t359.x=12;t359.y=1;t359.z=0;tiles.push_back(t359);TileEntity t360;t360.x=12;t360.y=10;t360.z=0;tiles.push_back(t360);TileEntity t361;t361.x=12;t361.y=12;t361.z=0;tiles.push_back(t361);TileEntity t362;t362.x=12;t362.y=13;t362.z=0;tiles.push_back(t362);TileEntity t363;t363.x=12;t363.y=14;t363.z=0;tiles.push_back(t363);TileEntity t364;t364.x=12;t364.y=16;t364.z=0;tiles.push_back(t364);TileEntity t365;t365.x=12;t365.y=19;t365.z=0;tiles.push_back(t365);TileEntity t366;t366.x=13;t366.y=-9;t366.z=0;tiles.push_back(t366);TileEntity t367;t367.x=13;t367.y=-5;t367.z=0;tiles.push_back(t367);TileEntity t368;t368.x=13;t368.y=0;t368.z=0;tiles.push_back(t368);TileEntity t369;t369.x=13;t369.y=1;t369.z=0;tiles.push_back(t369);TileEntity t370;t370.x=13;t370.y=12;t370.z=0;tiles.push_back(t370);TileEntity t371;t371.x=13;t371.y=13;t371.z=0;tiles.push_back(t371);TileEntity t372;t372.x=13;t372.y=19;t372.z=0;tiles.push_back(t372);TileEntity t373;t373.x=14;t373.y=-9;t373.z=0;tiles.push_back(t373);TileEntity t374;t374.x=14;t374.y=-5;t374.z=0;tiles.push_back(t374);TileEntity t375;t375.x=14;t375.y=-1;t375.z=0;tiles.push_back(t375);TileEntity t376;t376.x=14;t376.y=0;t376.z=0;tiles.push_back(t376);TileEntity t377;t377.x=14;t377.y=6;t377.z=0;tiles.push_back(t377);TileEntity t378;t378.x=14;t378.y=12;t378.z=0;tiles.push_back(t378);TileEntity t379;t379.x=14;t379.y=19;t379.z=0;tiles.push_back(t379);TileEntity t380;t380.x=15;t380.y=-9;t380.z=0;tiles.push_back(t380);TileEntity t381;t381.x=15;t381.y=-5;t381.z=0;tiles.push_back(t381);TileEntity t382;t382.x=15;t382.y=-1;t382.z=0;tiles.push_back(t382);TileEntity t383;t383.x=15;t383.y=1;t383.z=0;tiles.push_back(t383);TileEntity t384;t384.x=15;t384.y=8;t384.z=0;tiles.push_back(t384);TileEntity t385;t385.x=15;t385.y=19;t385.z=0;tiles.push_back(t385);TileEntity t386;t386.x=16;t386.y=-9;t386.z=0;tiles.push_back(t386);TileEntity t387;t387.x=16;t387.y=-5;t387.z=0;tiles.push_back(t387);TileEntity t388;t388.x=16;t388.y=-4;t388.z=0;tiles.push_back(t388);TileEntity t389;t389.x=16;t389.y=1;t389.z=0;tiles.push_back(t389);TileEntity t390;t390.x=16;t390.y=19;t390.z=0;tiles.push_back(t390);TileEntity t391;t391.x=17;t391.y=-9;t391.z=0;tiles.push_back(t391);TileEntity t392;t392.x=17;t392.y=1;t392.z=0;tiles.push_back(t392);TileEntity t393;t393.x=17;t393.y=19;t393.z=0;tiles.push_back(t393);TileEntity t394;t394.x=18;t394.y=-9;t394.z=0;tiles.push_back(t394);TileEntity t395;t395.x=18;t395.y=-8;t395.z=0;tiles.push_back(t395);TileEntity t396;t396.x=18;t396.y=-7;t396.z=0;tiles.push_back(t396);TileEntity t397;t397.x=18;t397.y=-6;t397.z=0;tiles.push_back(t397);TileEntity t398;t398.x=18;t398.y=-5;t398.z=0;tiles.push_back(t398);TileEntity t399;t399.x=18;t399.y=-4;t399.z=0;tiles.push_back(t399);TileEntity t400;t400.x=18;t400.y=-3;t400.z=0;tiles.push_back(t400);TileEntity t401;t401.x=18;t401.y=-2;t401.z=0;tiles.push_back(t401);TileEntity t402;t402.x=18;t402.y=-1;t402.z=0;tiles.push_back(t402);TileEntity t403;t403.x=18;t403.y=0;t403.z=0;tiles.push_back(t403);TileEntity t404;t404.x=18;t404.y=1;t404.z=0;tiles.push_back(t404);TileEntity t405;t405.x=18;t405.y=2;t405.z=0;tiles.push_back(t405);TileEntity t406;t406.x=18;t406.y=3;t406.z=0;tiles.push_back(t406);TileEntity t407;t407.x=18;t407.y=4;t407.z=0;tiles.push_back(t407);TileEntity t408;t408.x=18;t408.y=5;t408.z=0;tiles.push_back(t408);TileEntity t409;t409.x=18;t409.y=6;t409.z=0;tiles.push_back(t409);TileEntity t410;t410.x=18;t410.y=7;t410.z=0;tiles.push_back(t410);TileEntity t411;t411.x=18;t411.y=8;t411.z=0;tiles.push_back(t411);TileEntity t412;t412.x=18;t412.y=9;t412.z=0;tiles.push_back(t412);TileEntity t413;t413.x=18;t413.y=10;t413.z=0;tiles.push_back(t413);TileEntity t414;t414.x=18;t414.y=11;t414.z=0;tiles.push_back(t414);TileEntity t415;t415.x=18;t415.y=12;t415.z=0;tiles.push_back(t415);TileEntity t416;t416.x=18;t416.y=13;t416.z=0;tiles.push_back(t416);TileEntity t417;t417.x=18;t417.y=14;t417.z=0;tiles.push_back(t417);TileEntity t418;t418.x=18;t418.y=15;t418.z=0;tiles.push_back(t418);TileEntity t419;t419.x=18;t419.y=16;t419.z=0;tiles.push_back(t419);TileEntity t420;t420.x=18;t420.y=17;t420.z=0;tiles.push_back(t420);TileEntity t421;t421.x=18;t421.y=18;t421.z=0;tiles.push_back(t421);TileEntity t422;t422.x=18;t422.y=19;t422.z=0;tiles.push_back(t422);
		m_world->SetGravity(b2Vec2(0.0f, -10.0f));
		{
			std::list<TileEntity>::iterator it;
			for (it = tiles.begin(); it != tiles.end(); it++) {
				b2BodyDef bodyDef;
				bodyDef.type = b2_staticBody;
				bodyDef.position = b2Vec2(it->x, it->y);
				b2Body* body = m_world->CreateBody(&bodyDef);
				b2PolygonShape boxShape;
				boxShape.SetAsBox(halfSizeX, halfSizeY);
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = 1.0f;
				fixtureDef.friction = 0.3f;
				fixtureDef.restitution = 0.0f;
				body->CreateFixture(&fixtureDef);
			}

			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.position = b2Vec2(4, -4);
			bodyDef.fixedRotation = true;
			b2Body* body = m_world->CreateBody(&bodyDef);
			//����һ��2x2�������κ���
			b2PolygonShape boxShape;
			boxShape.SetAsBox(halfSizeX - 0.1, halfSizeY - 0.1);
			//Ϊ�����̬body���ò���
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxShape;
			fixtureDef.density = 1.0f; //�ܶ�
			fixtureDef.friction = 0.3f; //Ħ��ϵ��
			fixtureDef.restitution = 0.0f; //�ָ�ϵ��
			body->CreateFixture(&fixtureDef);
		}
	}

	void BeginContact(b2Contact* contact)  override {  
		b2BodyUserData bodyA = contact->GetFixtureA()->GetBody()->GetUserData();
		b2BodyUserData bodyB = contact->GetFixtureB()->GetBody()->GetUserData();

		std::cout << "contact";

	}
	
	void EndContact(b2Contact* contact)  override {  
	
	}


	static Test* Create()
	{
		return new Test3;
	}
};

static int Test3Index = RegisterTest("Benchmark", "Test3", Test3::Create);