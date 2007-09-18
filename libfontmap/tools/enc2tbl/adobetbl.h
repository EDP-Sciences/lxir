/* catdvi - get text from DVI files
   Copyright (C) 1999 Antti-Juhani Kaijanaho <gaia@iki.fi>
   Copyright (C) 2001 Bjoern Brill <brill@fs.math.uni-frankfurt.de>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef ADOBETBL_H
#define ADOBETBL_H

/* The following table is based on the Adobe Glyph List v. 1.2,
   available at
   http://partners.adobe.com/asn/developer/typeforum/glyphlist.txt The
   symbol names that begin with '$' are our additions. 
   This table has to be strcmp-sorted by glyph names! */

/* Adobe defines several glyphs in the BMP private use area U+E000..U+F8FF .
 * Catdvi references some of Adobes definitions and, additionally, uses
 * a few code points in the "high" (formerly "surrogate pairs") private use
 * area U+000F0000..U+0010FFFF :
 * - Single code points are allocated from U+0010FFFF downwards in no
 *   particular order.
 * - Lowest currently allocated: U+0010FFEF
 * - The "TeX Math Extension" font encoding is reproduced in its entirety,
 *   shifted to U+0010FF00..U+0010FF7F (with a reserved hole at
 *   U+0010FF30..U+0010FF43 where we would duplicate stuff that Adobe has
 *   already defined elsewhere).
 */

/* For box drawing glyphs (U+25xx), we use the following abbreviations:
 * prefix: bdraw
 * weights: light, heavy, singl, dbl
 * directions: up, dn, lf, rt, vert, horz
 */

#ifndef ADOBETBL_SHOULD_HAVE_CRUFTY_DUPLICATES

#define DUP1 ".DUP1"
#define DUP2 ".DUP2"

#else

#define DUP1
#define DUP2

#endif


static struct adobe2unicode_t adobe2unicode[] = {
	{ "$Amathbb", 0x0001d538 }, /* MATHEMATICAL DOUBLE-STRUCK CAPITAL A */
	{ "$Amathfrak", 0x0001d504 }, /* MATHEMATICAL FRAKTUR CAPITAL A */
	{ "$Amathscript", 0x0001d49c }, /* MATHEMATICAL SCRIPT CAPITAL A */
	{ "$Bmathbb", 0x0001d539 }, /* MATHEMATICAL DOUBLE-STRUCK CAPITAL B */
	{ "$Bmathfrak", 0x0001d505 }, /* MATHEMATICAL FRAKTUR CAPITAL B */
	{ "$Bscript", 0x212c }, /* SCRIPT CAPITAL B */
	{ "$Cdblstruck", 0x2102 }, /* DOUBLE-STRUCK CAPITAL C */
	{ "$Cfraktur", 0x212d }, /* BLACK-LETTER CAPITAL C */
	{ "$Cmathscript", 0x0001d49e }, /* MATHEMATICAL SCRIPT CAPITAL C */
        { "$Delta" , 0x0394 }, /* GREEK CAPITAL LETTER DELTA; distinguish Adobe duplicates */
	{ "$Dmathbb", 0x0001d53b }, /* MATHEMATICAL DOUBLE-STRUCK CAPITAL D */
	{ "$Dmathfrak", 0x0001d507 }, /* MATHEMATICAL FRAKTUR CAPITAL D */
	{ "$Dmathscript", 0x0001d49f }, /* MATHEMATICAL SCRIPT CAPITAL D */
        { "$DoubleS", 0x0010fffd }, /* PRIVATE SPACE CAPITAL GERMAN DOUBLE S */
	{ "$Emathbb", 0x0001d53c }, /* MATHEMATICAL DOUBLE-STRUCK CAPITAL E */
	{ "$Emathfrak", 0x0001d508 }, /* MATHEMATICAL FRAKTUR CAPITAL E */
	{ "$Escript", 0x2130 }, /* SCRIPT CAPITAL E */
        { "$Eurodblstroke", 0x0010fffa }, /* PRIVATE SPACE EURO SIGN DOUBLE STROKE */
        { "$Eurodblstrokecomb", 0x0010fff9 }, /* PRIVATE SPACE COMBINING EURO SIGN DOUBLE STROKE */
	{ "$Finv", 0x2132 }, /* TURNED CAPITAL F */
	{ "$Fmathbb", 0x0001d53d }, /* MATHEMATICAL DOUBLE-STRUCK CAPITAL F */
	{ "$Fmathfrak", 0x0001d509 }, /* MATHEMATICAL FRAKTUR CAPITAL F */
	{ "$Fscript", 0x2131 }, /* SCRIPT CAPITAL F */
	{ "$Game", 0x2141 }, /* TURNED SANS-SERIF CAPITAL G */
	{ "$Gmathbb", 0x0001d53e }, /* MATHEMATICAL DOUBLE-STRUCK CAPITAL G */
	{ "$Gmathfrak", 0x0001d50a }, /* MATHEMATICAL FRAKTUR CAPITAL G */
	{ "$Gmathscript", 0x0001d4a2 }, /* MATHEMATICAL SCRIPT CAPITAL G */
	{ "$Hdblstruck", 0x210d }, /* DOUBLE-STRUCK CAPITAL H */
	{ "$Hfraktur", 0x210c }, /* BLACK-LETTER CAPITAL H */
	{ "$Hscript", 0x210b }, /* SCRIPT CAPITAL H */
	{ "$Imathbb", 0x0001d540 }, /* MATHEMATICAL DOUBLE-STRUCK CAPITAL I */
	{ "$Iscript", 0x2110 }, /* SCRIPT CAPITAL I */
	{ "$Jmathbb", 0x0001d541 }, /* MATHEMATICAL DOUBLE-STRUCK CAPITAL J */
	{ "$Jmathfrak", 0x0001d50d }, /* MATHEMATICAL FRAKTUR CAPITAL J */
	{ "$Jmathscript", 0x0001d4a5 }, /* MATHEMATICAL SCRIPT CAPITAL J */
	{ "$Kmathbb", 0x0001d542 }, /* MATHEMATICAL DOUBLE-STRUCK CAPITAL K */
	{ "$Kmathfrak", 0x0001d50e }, /* MATHEMATICAL FRAKTUR CAPITAL K */
	{ "$Kmathscript", 0x0001d4a6 }, /* MATHEMATICAL SCRIPT CAPITAL K */
	{ "$Lmathbb", 0x0001d543 }, /* MATHEMATICAL DOUBLE-STRUCK CAPITAL L */
	{ "$Lmathfrak", 0x0001d50f }, /* MATHEMATICAL FRAKTUR CAPITAL L */
	{ "$Lscript", 0x2112 }, /* SCRIPT CAPITAL L */
        { "$Mho" , 0x2127 }, /* INVERTED OHM SIGN */
	{ "$Mmathbb", 0x0001d544 }, /* MATHEMATICAL DOUBLE-STRUCK CAPITAL M */
	{ "$Mmathfrak", 0x0001d510 }, /* MATHEMATICAL FRAKTUR CAPITAL M */
	{ "$Mscript", 0x2133 }, /* SCRIPT CAPITAL M */
	{ "$Ndblstruck", 0x2115 }, /* DOUBLE-STRUCK CAPITAL N */
        { "$Ng", 0x0010ffff }, /* PRIVATE SPACE LIGATURE CAPITAL NG */
	{ "$Nmathfrak", 0x0001d511 }, /* MATHEMATICAL FRAKTUR CAPITAL N */
	{ "$Nmathscript", 0x0001d4a9 }, /* MATHEMATICAL SCRIPT CAPITAL N */
        { "$Ohm" , 0x2126 }, /* OHM SIGN; distinguish Adobe duplicates */
	{ "$Omathbb", 0x0001d546 }, /* MATHEMATICAL DOUBLE-STRUCK CAPITAL O */
	{ "$Omathfrak", 0x0001d512 }, /* MATHEMATICAL FRAKTUR CAPITAL O */
	{ "$Omathscript", 0x0001d4aa }, /* MATHEMATICAL SCRIPT CAPITAL O */
        { "$Omega" , 0x03A9 }, /* GREEK CAPITAL LETTER OMEGA; distinguish Adobe duplicates */
	{ "$Pdblstruck", 0x2119 }, /* DOUBLE-STRUCK CAPITAL P */
	{ "$Pmathfrak", 0x0001d513 }, /* MATHEMATICAL FRAKTUR CAPITAL P */
	{ "$Pmathscript", 0x0001d4ab }, /* MATHEMATICAL SCRIPT CAPITAL P */
	{ "$Qdblstruck", 0x211a }, /* DOUBLE-STRUCK CAPITAL Q */
	{ "$Qmathfrak", 0x0001d514 }, /* MATHEMATICAL FRAKTUR CAPITAL Q */
	{ "$Qmathscript", 0x0001d4ac }, /* MATHEMATICAL SCRIPT CAPITAL Q */
	{ "$Rdblstruck", 0x211d }, /* DOUBLE-STRUCK CAPITAL R */
	{ "$Rscript", 0x211b }, /* SCRIPT CAPITAL R */
        { "$Scedilla" , 0x015E }, /* LATIN CAPITAL LETTER S WITH CEDILLA; distinguish Adobe duplicates */
	{ "$Smathbb", 0x0001d54a }, /* MATHEMATICAL DOUBLE-STRUCK CAPITAL S */
	{ "$Smathfrak", 0x0001d516 }, /* MATHEMATICAL FRAKTUR CAPITAL S */
	{ "$Smathscript", 0x0001d4ae }, /* MATHEMATICAL SCRIPT CAPITAL S */
        { "$Tcedilla", 0x0162 }, /* LATIN CAPITAL LETTER T WITH CEDILLA; distinguish Adobe duplicates */
        { "$Tcommaaccent", 0x021A }, /* LATIN CAPITAL LETTER T WITH COMMA BELOW; distinguish Adobe duplicates */
	{ "$Tmathbb", 0x0001d54b }, /* MATHEMATICAL DOUBLE-STRUCK CAPITAL T */
	{ "$Tmathfrak", 0x0001d517 }, /* MATHEMATICAL FRAKTUR CAPITAL T */
	{ "$Tmathscript", 0x0001d4af }, /* MATHEMATICAL SCRIPT CAPITAL T */
	{ "$Umathbb", 0x0001d54c }, /* MATHEMATICAL DOUBLE-STRUCK CAPITAL U */
	{ "$Umathfrak", 0x0001d518 }, /* MATHEMATICAL FRAKTUR CAPITAL U */
	{ "$Umathscript", 0x0001d4b0 }, /* MATHEMATICAL SCRIPT CAPITAL U */
	{ "$Vmathbb", 0x0001d54d }, /* MATHEMATICAL DOUBLE-STRUCK CAPITAL V */
	{ "$Vmathfrak", 0x0001d519 }, /* MATHEMATICAL FRAKTUR CAPITAL V */
	{ "$Vmathscript", 0x0001d4b1 }, /* MATHEMATICAL SCRIPT CAPITAL V */
	{ "$Wmathbb", 0x0001d54e }, /* MATHEMATICAL DOUBLE-STRUCK CAPITAL W */
	{ "$Wmathfrak", 0x0001d51a }, /* MATHEMATICAL FRAKTUR CAPITAL W */
	{ "$Wmathscript", 0x0001d4b2 }, /* MATHEMATICAL SCRIPT CAPITAL W */
	{ "$Xmathbb", 0x0001d54f }, /* MATHEMATICAL DOUBLE-STRUCK CAPITAL X */
	{ "$Xmathfrak", 0x0001d51b }, /* MATHEMATICAL FRAKTUR CAPITAL X */
	{ "$Xmathscript", 0x0001d4b3 }, /* MATHEMATICAL SCRIPT CAPITAL X */
	{ "$Ymathbb", 0x0001d550 }, /* MATHEMATICAL DOUBLE-STRUCK CAPITAL Y */
	{ "$Ymathfrak", 0x0001d51c }, /* MATHEMATICAL FRAKTUR CAPITAL Y */
	{ "$Ymathscript", 0x0001d4b4 }, /* MATHEMATICAL SCRIPT CAPITAL Y */
        { "$ZWNJ" , 0x200c }, /* ZERO WIDTH NON-JOINER */
	{ "$Zdblstruck", 0x2124 }, /* DOUBLE-STRUCK CAPITAL Z */
	{ "$Zfraktur", 0x2128 }, /* BLACK-LETTER CAPITAL Z */
	{ "$Zmathscript", 0x0001d4b5 }, /* MATHEMATICAL SCRIPT CAPITAL Z */
    	{ "$acutemodifier", 0x02ca }, /* MODIFIER LETTER ACUTE ACCENT */
	{ "$amathbb", 0x0001d552 }, /* MATHEMATICAL DOUBLE-STRUCK SMALL A */
	{ "$amathfrak", 0x0001d51e }, /* MATHEMATICAL FRAKTUR SMALL A */
	{ "$angbracketleftBig", 0x0010ff44 }, /* PRIVATE SPACE VERY LARGE LEFT-POINTING ANGLE BRACKET */
	{ "$angbracketleftBigg", 0x0010ff2a }, /* PRIVATE SPACE VERY HUGE LEFT-POINTING ANGLE BRACKET */
	{ "$angbracketleftbig", 0x0010ff0a }, /* PRIVATE SPACE LARGE LEFT-POINTING ANGLE BRACKET */
	{ "$angbracketleftbigg", 0x0010ff1c }, /* PRIVATE SPACE HUGE LEFT-POINTING ANGLE BRACKET */
	{ "$angbracketrightBig", 0x0010ff45 }, /* PRIVATE SPACE VERY LARGE RIGHT-POINTING ANGLE BRACKET */
	{ "$angbracketrightBigg", 0x0010ff2b }, /* PRIVATE SPACE VERY HUGE RIGHT-POINTING ANGLE BRACKET */
	{ "$angbracketrightbig", 0x0010ff0b }, /* PRIVATE SPACE LARGE RIGHT-POINTING ANGLE BRACKET */
	{ "$angbracketrightbigg", 0x0010ff1d }, /* PRIVATE SPACE HUGE RIGHT-POINTING ANGLE BRACKET */
    	{ "$angleleftmath", 0x27e8 }, /* MATHEMATICAL LEFT ANGLE BRACKET */
    	{ "$anglerightmath", 0x27e9 }, /* MATHEMATICAL RIGHT ANGLE BRACKET */
    	{ "$ankh", 0x2625 }, /* ANKH */
    	{ "$answerphone", 0x2315 }, /* TELEPHONE RECORDER */
	{ "$approxequal1", 0x224a }, /* ALMOST EQUAL OR EQUAL TO */
    	{ "$aquarius", 0x2652 }, /* AQUARIUS */
    	{ "$aries", 0x2648 }, /* ARIES */
	{ "$arrowanticlockwise", 0x21ba }, /* ANTICLOCKWISE OPEN CIRCLE ARROW */
	{ "$arrowbt", 0x0010ff79 }, /* PRIVATE SPACE ARROW BOTTOM */
	{ "$arrowclockwise", 0x21bb }, /* CLOCKWISE OPEN CIRCLE ARROW */
	{ "$arrowdblbt", 0x0010ff7f }, /* PRIVATE SPACE DOUBLE ARROW BOTTOM */
	{ "$arrowdbltp", 0x0010ff7e }, /* PRIVATE SPACE DOUBLE ARROW TOP */
    	{ "$arrowdblupdn", 0x21d5 }, /* UP DOWN DOUBLE ARROW */
    	{ "$arrowheaddown", 0x2304 }, /* DOWN ARROWHEAD */
    	{ "$arrowheadup", 0x2303 }, /* UP ARROWHEAD */
    	{ "$arrownortheast", 0x2197 }, /* NORTH EAST ARROW */
    	{ "$arrownorthwest", 0x2196 }, /* NORTH WEST ARROW */
	{ "$arrowsdowndown", 0x21ca }, /* DOWNWARDS PAIRED ARROWS */
	{ "$arrowsleftleft", 0x21c7 }, /* LEFTWARDS PAIRED ARROWS */
	{ "$arrowsleftright", 0x21c6 }, /* LEFTWARDS ARROW OVER RIGHTWARDS ARROW */
    	{ "$arrowsoutheast", 0x2198 }, /* SOUTH EAST ARROW */
    	{ "$arrowsouthwest", 0x2199 }, /* SOUTH WEST ARROW */
	{ "$arrowsrightleft", 0x21c4 }, /* RIGHTWARDS ARROW OVER LEFTWARDS ARROW */
	{ "$arrowsrightright", 0x21c9 }, /* RIGHTWARDS PAIRED ARROWS */
	{ "$arrowsupup", 0x21c8 }, /* UPWARDS PAIRED ARROWS */
	{ "$arrowtp", 0x0010ff78 }, /* PRIVATE SPACE ARROW TOP */
	{ "$arrowtripleleft", 0x21da }, /* LEFTWARDS TRIPLE ARROW */
	{ "$arrowtripleright", 0x21db }, /* RIGHTWARDS TRIPLE ARROW */
	{ "$arrowuptipleft", 0x21b0 }, /* UPWARDS ARROW WITH TIP LEFTWARDS */
	{ "$arrowuptipright", 0x21b1 }, /* UPWARDS ARROW WITH TIP RIGHTWARDS */
	{ "$arrowvertexdbl", 0x0010ff77 }, /* PRIVATE SPACE DOUBLE ARROW VERTICAL EXTENDER */
	{ "$asterisklow", 0x204e }, /* LOW ASTERISK */
	{ "$backslashBig", 0x0010ff2f }, /* PRIVATE SPACE VERY LARGE REVERSE SOLIDUS */
	{ "$backslashBigg", 0x0010ff2d }, /* PRIVATE SPACE VERY HUGE REVERSE SOLIDUS */
	{ "$backslashbig", 0x0010ff0f }, /* PRIVATE SPACE LARGE REVERSE SOLIDUS */
	{ "$backslashbigg", 0x0010ff1f }, /* PRIVATE SPACE HUGE REVERSE SOLIDUS */
	{ "$baht", 0x0e3f }, /* THAI CURRENCY SYMBOL BAHT */
	{ "$bardbl", 0x2016 }, /* DOUBLE VERTICAL LINE */
	{ "$barwedge", 0x22bc }, /* NAND */
        { "$bdrawlightdn", 0x2577 }, /* BOX DRAWINGS LIGHT DOWN */
        { "$bdrawlightdnhorz", 0x252c }, /* BOX DRAWINGS LIGHT DOWN AND HORIZONTAL; Adobe has this as "SF060000" */
        { "$bdrawlightdnlf", 0x2510 }, /* BOX DRAWINGS LIGHT DOWN AND LEFT; Adobe has this as "SF030000" */
        { "$bdrawlightdnrt", 0x250c }, /* BOX DRAWINGS LIGHT DOWN AND RIGHT; Adobe has this as "SF010000" */
        { "$bdrawlighthorz", 0x2500 }, /* BOX DRAWINGS LIGHT HORIZONTAL; Adobe has this as "SF100000" */
        { "$bdrawlightlf", 0x2574 }, /* BOX DRAWINGS LIGHT LEFT */
        { "$bdrawlightrt", 0x2576 }, /* BOX DRAWINGS LIGHT RIGHT */
        { "$bdrawlightup", 0x2575 }, /* BOX DRAWINGS LIGHT UP */
        { "$bdrawlightuphorz", 0x2534 }, /* BOX DRAWINGS LIGHT UP AND HORIZONTAL; Adobe has this as "SF070000" */
        { "$bdrawlightuplf", 0x2518 }, /* BOX DRAWINGS LIGHT UP AND LEFT; Adobe has this as "SF040000" */
        { "$bdrawlightuprt", 0x2514 }, /* BOX DRAWINGS LIGHT UP AND RIGHT; Adobe has this as "SF020000" */
        { "$bdrawlightvert", 0x2502 }, /* BOX DRAWINGS LIGHT VERTICAL; Adobe has this as "SF110000" */
        { "$bdrawlightverthorz", 0x253c }, /* BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL; Adobe has this as "SF050000" */
        { "$bdrawlightvertlf", 0x2524 }, /* BOX DRAWINGS LIGHT VERTICAL AND LEFT; Adobe has this as "SF090000" */
        { "$bdrawlightvertrt", 0x251c }, /* BOX DRAWINGS LIGHT VERTICAL AND RIGHT; Adobe has this as "SF080000" */
	{ "$because", 0x2235 }, /* BECAUSE */
	{ "$beth", 0x2136 }, /* BET SYMBOL */
	{ "$between", 0x226c }, /* BETWEEN */
	{ "$biohazard", 0x2623 }, /* BIOHAZARD SIGN */
	{ "$blank", 0x2422 }, /* BLANK SYMBOL */
	{ "$bmathbb", 0x0001d553 }, /* MATHEMATICAL DOUBLE-STRUCK SMALL B */
	{ "$bmathfrak", 0x0001d51f }, /* MATHEMATICAL FRAKTUR SMALL B */
    	{ "$bowtie", 0x22c8 }, /* BOWTIE */
    	{ "$box", 0x25a1 }, /* WHITE SQUARE */
    	{ "$boxdot", 0x22a1 }, /* SQUARED DOT OPERATOR */
    	{ "$boxminus", 0x229f }, /* SQUARED MINUS */
    	{ "$boxmultiply", 0x22a0 }, /* SQUARED TIMES */
    	{ "$boxplus", 0x229e }, /* SQUARED PLUS */
    	{ "$braceex", 0x23aa }, /* CURLY BRACKET EXTENSION; not Adobe's private space version */
	{ "$bracehtipdownleft", 0x0010ff7a }, /* PRIVATE SPACE UPWARDS-POINTING CURLY BRACKET LEFT TIP */
	{ "$bracehtipdownright", 0x0010ff7b }, /* PRIVATE SPACE UPWARDS-POINTING CURLY BRACKET RIGHT TIP */
	{ "$bracehtipupleft", 0x0010ff7c }, /* PRIVATE SPACE DOWNWARDS-POINTING CURLY BRACKET LEFT TIP */
	{ "$bracehtipupright", 0x0010ff7d }, /* PRIVATE SPACE DOWNWARDS-POINTING CURLY BRACKET RIGHT TIP */
	{ "$braceleftBig", 0x0010ff6e }, /* PRIVATE SPACE VERY LARGE LEFT CURLY BRACKET */
	{ "$braceleftBigg", 0x0010ff28 }, /* PRIVATE SPACE VERY HUGE LEFT CURLY BRACKET */
	{ "$braceleftbig", 0x0010ff08 }, /* PRIVATE SPACE LARGE LEFT CURLY BRACKET */
	{ "$braceleftbigg", 0x0010ff1a }, /* PRIVATE SPACE HUGE LEFT CURLY BRACKET */
    	{ "$braceleftbt", 0x23a9 }, /* LEFT CURLY BRACKET LOWER HOOK; not Adobe's private space version */
    	{ "$braceleftmid", 0x23a8 }, /* LEFT CURLY BRACKET MIDDLE PIECE; not Adobe's private space version */
    	{ "$bracelefttp", 0x23a7 }, /* LEFT CURLY BRACKET UPPER HOOK; not Adobe's private space version */
    	{ "$braceleftwhite", 0x2983 }, /* LEFT WHITE CURLY BRACKET */
	{ "$bracerightBig", 0x0010ff6f }, /* PRIVATE SPACE VERY LARGE RIGHT CURLY BRACKET */
	{ "$bracerightBigg", 0x0010ff29 }, /* PRIVATE SPACE VERY HUGE RIGHT CURLY BRACKET */
	{ "$bracerightbig", 0x0010ff09 }, /* PRIVATE SPACE LARGE RIGHT CURLY BRACKET */
	{ "$bracerightbigg", 0x0010ff1b }, /* PRIVATE SPACE HUGE RIGHT CURLY BRACKET */
    	{ "$bracerightbt", 0x23ad }, /* RIGHT CURLY BRACKET LOWER HOOK; not Adobe's private space version */
    	{ "$bracerightmid", 0x23ac }, /* RIGHT CURLY BRACKET MIDDLE PIECE; not Adobe's private space version */
    	{ "$bracerighttp", 0x23ab }, /* RIGHT CURLY BRACKET UPPER HOOK; not Adobe's private space version */
    	{ "$bracerightwhite", 0x2984 }, /* RIGHT WHITE CURLY BRACKET */
	{ "$bracketleftBig", 0x0010ff68 }, /* PRIVATE SPACE VERY LARGE LEFT SQUARE BRACKET */
	{ "$bracketleftBigg", 0x0010ff22 }, /* PRIVATE SPACE VERY HUGE LEFT SQUARE BRACKET */
	{ "$bracketleftbig", 0x0010ff02 }, /* PRIVATE SPACE LARGE LEFT SQUARE BRACKET */
	{ "$bracketleftbigg", 0x0010ff14 }, /* PRIVATE SPACE HUGE LEFT SQUARE BRACKET */
    	{ "$bracketleftbt", 0x23a3 }, /* LEFT SQUARE BRACKET LOWER CORNER; not Adobe's private space version */
    	{ "$bracketleftex", 0x23a2 }, /* LEFT SQUARE BRACKET EXTENSION; not Adobe's private space version */
    	{ "$bracketlefttp", 0x23a1 }, /* LEFT SQUARE BRACKET UPPER CORNER; not Adobe's private space version */
	{ "$bracketleftwhite", 0x301a }, /* LEFT WHITE SQUARE BRACKET */
    	{ "$bracketleftwhitemath", 0x27e6 }, /* MATHEMATICAL LEFT WHITE SQUARE BRACKET */
	{ "$bracketrightBig", 0x0010ff69 }, /* PRIVATE SPACE VERY LARGE RIGHT SQUARE BRACKET */
	{ "$bracketrightBigg", 0x0010ff23 }, /* PRIVATE SPACE VERY HUGE RIGHT SQUARE BRACKET */
	{ "$bracketrightbig", 0x0010ff03 }, /* PRIVATE SPACE LARGE RIGHT SQUARE BRACKET */
	{ "$bracketrightbigg", 0x0010ff15 }, /* PRIVATE SPACE HUGE RIGHT SQUARE BRACKET */
    	{ "$bracketrightbt", 0x23a6 }, /* RIGHT SQUARE BRACKET LOWER CORNER; not Adobe's private space version */
    	{ "$bracketrightex", 0x23a5 }, /* RIGHT SQUARE BRACKET EXTENSION; not Adobe's private space version */
    	{ "$bracketrighttp", 0x23a4 }, /* RIGHT SQUARE BRACKET UPPER CORNER; not Adobe's private space version */
	{ "$bracketrightwhite", 0x301b }, /* RIGHT WHITE SQUARE BRACKET */
    	{ "$bracketrightwhitemath", 0x27e7 }, /* MATHEMATICAL RIGHT WHITE SQUARE BRACKET */
    	{ "$brevecomb", 0x0306 }, /* COMBINING BREVE */
        { "$bulletmath" , 0x2219 }, /* BULLET OPERATOR; distinguish Adobe duplicates */
	{ "$bumpdentequal", 0x224e }, /* GEOMETRICALLY EQUIVALENT TO */
	{ "$bumpequal", 0x224f }, /* DIFFERENCE BETWEEN */
    	{ "$cancer", 0x264b }, /* CANCER */
    	{ "$capricorn", 0x2651 }, /* CAPRICORN */
    	{ "$caroncomb", 0x030c }, /* COMBINING CARON */
    	{ "$cedillacomb", 0x0327 }, /* COMBINING CEDILLA */
    	{ "$ceilingleft", 0x2308 }, /* LEFT CEILING */
	{ "$ceilingleftBig", 0x0010ff6c }, /* PRIVATE SPACE VERY LARGE LEFT CEILING */
	{ "$ceilingleftBigg", 0x0010ff26 }, /* PRIVATE SPACE VERY HUGE LEFT CEILING */
	{ "$ceilingleftbig", 0x0010ff06 }, /* PRIVATE SPACE LARGE LEFT CEILING */
	{ "$ceilingleftbigg", 0x0010ff18 }, /* PRIVATE SPACE HUGE LEFT CEILING */
    	{ "$ceilingright", 0x2309 }, /* RIGHT CEILING */
	{ "$ceilingrightBig", 0x0010ff6d }, /* PRIVATE SPACE VERY LARGE RIGHT CEILING */
	{ "$ceilingrightBigg", 0x0010ff27 }, /* PRIVATE SPACE VERY HUGE RIGHT CEILING */
	{ "$ceilingrightbig", 0x0010ff07 }, /* PRIVATE SPACE LARGE RIGHT CEILING */
	{ "$ceilingrightbigg", 0x0010ff19 }, /* PRIVATE SPACE HUGE RIGHT CEILING */
        { "$celsius" , 0x2103 }, /* DEGREE CELSIUS */
    	{ "$checkedbox", 0x2611 }, /* BALLOT BOX WITH CHECK */
    	{ "$checkmark", 0x2713 }, /* CHECK MARK */
        { "$circleA", 0x24b6 }, /* CIRCLED LATIN CAPITAL LETTER A */
        { "$circleB", 0x24b7 }, /* CIRCLED LATIN CAPITAL LETTER B */
        { "$circleC", 0x24b8 }, /* CIRCLED LATIN CAPITAL LETTER C */
        { "$circleD", 0x24b9 }, /* CIRCLED LATIN CAPITAL LETTER D */
        { "$circleE", 0x24ba }, /* CIRCLED LATIN CAPITAL LETTER E */
        { "$circleF", 0x24bb }, /* CIRCLED LATIN CAPITAL LETTER F */
        { "$circleG", 0x24bc }, /* CIRCLED LATIN CAPITAL LETTER G */
        { "$circleH", 0x24bd }, /* CIRCLED LATIN CAPITAL LETTER H */
        { "$circleI", 0x24be }, /* CIRCLED LATIN CAPITAL LETTER I */
        { "$circleJ", 0x24bf }, /* CIRCLED LATIN CAPITAL LETTER J */
        { "$circleK", 0x24c0 }, /* CIRCLED LATIN CAPITAL LETTER K */
        { "$circleL", 0x24c1 }, /* CIRCLED LATIN CAPITAL LETTER L */
        { "$circleM", 0x24c2 }, /* CIRCLED LATIN CAPITAL LETTER M */
        { "$circleN", 0x24c3 }, /* CIRCLED LATIN CAPITAL LETTER N */
        { "$circleO", 0x24c4 }, /* CIRCLED LATIN CAPITAL LETTER O */
        { "$circleP", 0x24c5 }, /* CIRCLED LATIN CAPITAL LETTER P */
        { "$circleQ", 0x24c6 }, /* CIRCLED LATIN CAPITAL LETTER Q */
        { "$circleR", 0x24c7 }, /* CIRCLED LATIN CAPITAL LETTER R */
        { "$circleS", 0x24c8 }, /* CIRCLED LATIN CAPITAL LETTER S */
        { "$circleT", 0x24c9 }, /* CIRCLED LATIN CAPITAL LETTER T */
        { "$circleU", 0x24ca }, /* CIRCLED LATIN CAPITAL LETTER U */
        { "$circleV", 0x24cb }, /* CIRCLED LATIN CAPITAL LETTER V */
        { "$circleW", 0x24cc }, /* CIRCLED LATIN CAPITAL LETTER W */
        { "$circleX", 0x24cd }, /* CIRCLED LATIN CAPITAL LETTER X */
        { "$circleY", 0x24ce }, /* CIRCLED LATIN CAPITAL LETTER Y */
        { "$circleZ", 0x24cf }, /* CIRCLED LATIN CAPITAL LETTER Z */
    	{ "$circleasterisk", 0x229b }, /* CIRCLED ASTERISK OPERATOR */
    	{ "$circlecomb", 0x20dd }, /* COMBINING ENCLOSING CIRCLE */
    	{ "$circledash", 0x229d }, /* CIRCLED DASH */
    	{ "$circledivide", 0x2298 }, /* CIRCLED DIVISION SLASH */
    	{ "$circledot", 0x2299 }, /* CIRCLED DOT OPERATOR */
	{ "$circledotdisplay", 0x0010ff4b }, /* PRIVATE SPACE DISPLAY MATH CIRCLED DOT OPERATOR */
	{ "$circledottext", 0x0010ff4a }, /* PRIVATE SPACE INLINE MATH CIRCLED DOT OPERATOR */
    	{ "$circleequal", 0x229c }, /* CIRCLED EQUALS */
    	{ "$circlelarge", 0x25ef }, /* LARGE CIRCLE */
    	{ "$circleminus", 0x2296 }, /* CIRCLED MINUS */
	{ "$circlemultiplydisplay", 0x0010ff4f }, /* PRIVATE SPACE DISPLAY MATH CIRCLED TIMES */
	{ "$circlemultiplytext", 0x0010ff4e }, /* PRIVATE SPACE INLINE MATH CIRCLED TIMES */
	{ "$circleplusdisplay", 0x0010ff4d }, /* PRIVATE SPACE DISPLAY MATH CIRCLED PLUS */
	{ "$circleplustext", 0x0010ff4c }, /* PRIVATE SPACE INLINE MATH CIRCLED PLUS */
    	{ "$circlering", 0x229a }, /* CIRCLED RING OPERATOR */
    	{ "$circumflexcomb", 0x0302 }, /* COMBINING CIRCUMFLEX ACCENT */
	{ "$cmathbb", 0x0001d554 }, /* MATHEMATICAL DOUBLE-STRUCK SMALL C */
	{ "$cmathfrak", 0x0001d520 }, /* MATHEMATICAL FRAKTUR SMALL C */
	{ "$complement", 0x2201 }, /* COMPLEMENT */
	{ "$contintegral", 0x222e }, /* CONTOUR INTEGRAL */
	{ "$contintegraldisplay", 0x0010ff49 }, /* PRIVATE SPACE DISPLAY MATH CONTOUR INTEGRAL */
	{ "$contintegraltext", 0x0010ff48 }, /* PRIVATE SPACE INLINE MATH CONTOUR INTEGRAL */
    	{ "$coproduct", 0x2210 }, /* N-ARY COPRODUCT */
	{ "$coproductdisplay", 0x0010ff61 }, /* PRIVATE SPACE DISPLAY MATH N-ARY COPRODUCT */
	{ "$coproducttext", 0x0010ff60 }, /* PRIVATE SPACE INLINE MATH N-ARY COPRODUCT */
    	{ "$corresponds", 0x2259 }, /* ESTIMATES */
    	{ "$crossedbox", 0x2612 }, /* BALLOT BOX WITH X */
	{ "$curlyvee", 0x22ce }, /* CURLY LOGICAl OR */
	{ "$curlywedge", 0x22cf }, /* CURLY LOGICAL AND */
	{ "$curvearrowleft", 0x21b6 }, /* ANTICLOCKWISE TOP SEMICIRCLE ARROW */
	{ "$curvearrowright", 0x21b7 }, /* CLOCKWISE TOP SEMICIRCLE ARROW */
	{ "$daleth", 0x2138 }, /* DALET SYMBOL */
	{ "$deltaequal", 0x225c }, /* DELTA EQUAL TO */
    	{ "$diamondmath", 0x22c4 }, /* DIAMOND OPERATOR */
    	{ "$diamondshape", 0x25c7 }, /* WHITE DIAMOND */
    	{ "$diamondwhite", 0x2662 }, /* WHITE DIAMOND SUIT */
    	{ "$dieresiscomb", 0x0308 }, /* COMBINING DIAERESIS */
	{ "$digamma", 0x03dc }, /* GREEK LETTER DIGAMMA */
    	{ "$dingbatarrow2794", 0x2794 }, /* HEAVY WIDE-HEADED RIGHTWARDS ARROW */
    	{ "$discount", 0x2052 }, /* COMMERCIAL MINUS SIGN */
	{ "$divideontimes", 0x22c7 }, /* DIVISION TIMES */
	{ "$divides", 0x2223 }, /* DIVIDES */
        { "$dividesnot", 0x2224 }, /* DOES NOT DIVIDE */
	{ "$divisionbackslashbig", 0x29f9 }, /* BIG REVERSE SOLIDUS */
    	{ "$divisionslash", 0x2215 }, /* DIVISION SLASH; distinguish Adobe duplicates */
	{ "$divisionslashbig", 0x29f8 }, /* BIG SOLIDUS */
	{ "$dmathbb", 0x0001d555 }, /* MATHEMATICAL DOUBLE-STRUCK SMALL D */
	{ "$dmathfrak", 0x0001d521 }, /* MATHEMATICAL FRAKTUR SMALL D */
    	{ "$dotaccentcomb", 0x0307 }, /* COMBINING DOT ABOVE */
	{ "$dotequaldot", 0x2251 }, /* GEOMETRICALLY EQUAL TO */
    	{ "$dotplus", 0x2214 }, /* DOT PLUS */
	{ "$doublebarwedge", 0x2a5e }, /* LOGICAL AND WITH DOUBLE OVERBAR */
    	{ "$earth", 0x2641 }, /* EARTH */
	{ "$eightmathbb", 0x0001de0 }, /* MATHEMATICAL DOUBLE-STRUCK DIGIT EIGHT */
	{ "$emathbb", 0x0001d556 }, /* MATHEMATICAL DOUBLE-STRUCK SMALL E */
	{ "$emathfrak", 0x0001d522 }, /* MATHEMATICAL FRAKTUR SMALL E */
    	{ "$envelope", 0x2709 }, /* ENVELOPE */
    	{ "$epsilon1", 0x03f5 }, /* GREEK LUNATE EPSILON SYMBOL */
	{ "$epsilon1inv", 0x03f6 }, /* GREEK REVERSED LUNATE EPSILON SYMBOL */
	{ "$equalfollows", 0x22df }, /* EQUAl TO OR SUCCEEDS */
	{ "$equalprecedes", 0x22de }, /* EQUAL TO OR PRECEDES */
	{ "$equalsimilar", 0x2242 }, /* MINUS TILDE */
	{ "$equalslantedgreater", 0x2a96 }, /* SLANTED EQUAl TO OR GREATER-THAN */
	{ "$equalslantedless", 0x2a95 }, /* SLANTED EQUAl TO OR LESS-THAN */
    	{ "$equivasymptotic", 0x224d }, /* EQUIVALENT TO */
	{ "$fallingdotsequal", 0x2252 }, /* APPROXIMATELY EQUAL TO OR THE IMAGE OF */
        { "$filledboxsmall", 0x25aa }, /* BLACK SMALL SQUARE; Adobe has this as "H18543" */
    	{ "$filledcircle", 0x25cf }, /* BLACK CIRCLE; Adobe has this as "H18533" */
	{ "$filledlozenge", 0x29eb }, /* BLACK LOZENGE */
    	{ "$fist", 0x261e }, /* WHITE RIGHT POINTING INDEX */
	{ "$fivemathbb", 0x0001d7dd }, /* MATHEMATICAL DOUBLE-STRUCK DIGIT FIVE */
    	{ "$flat", 0x266d }, /* MUSIC FLAT SIGN */
    	{ "$floorleft", 0x230a }, /* LEFT FLOOR */
	{ "$floorleftBig", 0x0010ff6a }, /* PRIVATE SPACE VERY LARGE LEFT FLOOR */
	{ "$floorleftBigg", 0x0010ff24 }, /* PRIVATE SPACE VERY HUGE LEFT FLOOR */
	{ "$floorleftbig", 0x0010ff04 }, /* PRIVATE SPACE LARGE LEFT FLOOR */
	{ "$floorleftbigg", 0x0010ff16 }, /* PRIVATE SPACE HUGE LEFT FLOOR */
    	{ "$floorright", 0x230b }, /* RIGHT FLOOR */
	{ "$floorrightBig", 0x0010ff6b }, /* PRIVATE SPACE VERY LARGE RIGHT FLOOR */
	{ "$floorrightBigg", 0x0010ff25 }, /* PRIVATE SPACE VERY HUGE RIGHT FLOOR */
	{ "$floorrightbig", 0x0010ff05 }, /* PRIVATE SPACE LARGE RIGHT FLOOR */
	{ "$floorrightbigg", 0x0010ff17 }, /* PRIVATE SPACE HUGE RIGHT FLOOR */
	{ "$fmathbb", 0x0001d557 }, /* MATHEMATICAL DOUBLE-STRUCK SMALL F */
	{ "$fmathfrak", 0x0001d523 }, /* MATHEMATICAL FRAKTUR SMALL F */
    	{ "$follows", 0x227b }, /* SUCCEEDS */
	{ "$followsapprox", 0x2ab8 }, /* SUCCEEDS ABOVE ALMOST EQUAL TO */
    	{ "$followsequal", 0x227d }, /* SUCCEEDS OR EQUAL TO */
	{ "$followsnot", 0x2281 }, /* DOES NOT SUCCEED */
	{ "$followsnotapprox", 0x2aba }, /* SUCCEEDS ABOVE NOT ALMOST EQUAL TO */
	{ "$followsnotequal", 0x2ab6 }, /* SUCCEEDS ABOVE NOT EQUAL TO */
	{ "$followsnotsimilar", 0x22e9 }, /* SUCCEEDS BUT NOT EQUIVALENT TO */
	{ "$followssimilar", 0x227f }, /* SUCCEEDS OR EQUIVALENT TO */
	{ "$forces", 0x22a9 }, /* FORCES */
	{ "$forcesnot", 0x22ae }, /* DOES NOT FORCE */
	{ "$forcestriplevbar", 0x22aa }, /* TRIPLE VERTICAL BAR RIGHT TURNSTILE */
	{ "$fourmathbb", 0x0001d7dc }, /* MATHEMATICAL DOUBLE-STRUCK DIGIT FOUR */
    	{ "$fractionslash", 0x2044 }, /* FRACTION SLASH; distinguish Adobe duplicates */
    	{ "$frown", 0x2322 }, /* FROWN */
    	{ "$frownface", 0x2639 }, /* WHITE FROWNING FACE */
    	{ "$gemini", 0x264a }, /* GEMINI */
	{ "$gimel", 0x2137 }, /* GIMEL SYMBOL */
	{ "$gmathbb", 0x0001d558 }, /* MATHEMATICAL DOUBLE-STRUCK SMALL G */
	{ "$gmathfrak", 0x0001d524 }, /* MATHEMATICAL FRAKTUR SMALL G */
    	{ "$gravemodifier", 0x02cb }, /* MODIFIER LETTER GRAVE ACCENT */
	{ "$greaterapprox", 0x2a86 }, /* GREATER-THAN OR APPROXIMATE */
	{ "$greaterdot", 0x22d7 }, /* GREATER-THAN WITH DOT */
	{ "$greaterequal2", 0x2267 }, /* GREATER-THAN OVER EQUAL TO */
	{ "$greaterequalless", 0x22db }, /* GREATER-THAN EQUAL TO OR LESS-THAN */
	{ "$greaterequallessstacked", 0x2a8c }, /* GREATER-THAN ABOVE DOUBLE-LINE EQUAL ABOVE LESS-THAN */
	{ "$greaterequalslanted", 0x2a7e }, /* GREATER-THAN OR SLANTED EQUAL TO */
	{ "$greaterless", 0x2277 }, /* GREATER-THAN OR LESS-THAN */
    	{ "$greatermuch", 0x226b }, /* MUCH GREATER-THAN */
	{ "$greaternotapprox", 0x2a8a }, /* GREATER-THAN AND NOT APPROXIMATE */
	{ "$greaternotequal", 0x2269 }, /* GREATER-THAN BUT NOT EQUAL TO */
	{ "$greaternotequal1", 0x2a88 }, /* GREATER-THAN AND SINGLE-LINE NOT EQUAL TO */
	{ "$greaternotsimilar", 0x22e7 }, /* GREATER-THAN BUT NOT EQUIVALENT TO */
	{ "$greatersimilar", 0x2273 }, /* GREATER-THAN OR EQUIVALENT TO */
    	{ "$greaterverymuch", 0x22d9 }, /* VERY MUCH GREATER-THAN */
	{ "$harpoondownbarbleft", 0x21c3 }, /* DOWNWARDS HARPOON WITH BARB LEFTWARDS */
	{ "$harpoondownbarbright", 0x21c2 }, /* DOWNWARDS HARPOON WITH BARB RIGHTWARDS */
    	{ "$harpoonleftbarbdown", 0x21bd }, /* LEFTWARDS HARPOON WITH BARB DOWNWARDS */
    	{ "$harpoonleftbarbup", 0x21bc }, /* LEFTWARDS HARPOON WITH BARB UPWARDS */
    	{ "$harpoonrightbarbdown", 0x21c1 }, /* RIGHTWARDS HARPOON WITH BARB DOWNWARDS */
    	{ "$harpoonrightbarbup", 0x21c0 }, /* RIGHTWARDS HARPOON WITH BARB UPWARDS */
	{ "$harpoonsleftright", 0x21cb }, /* LEFTWARDS HARPOON OVER RIGHTWARDS HARPOON */
	{ "$harpoonsrightleft", 0x21cc }, /* RIGHTWARDS HARPOON OVER LEFTWARDS HARPOON */
	{ "$harpoonupbarbleft", 0x21bf }, /* UPWARDS HARPOON WITH BARB LEFTWARDS */
	{ "$harpoonupbarbright", 0x21be }, /* UPWARDS HARPOON WITH BARB RIGHTWARDS */
	{ "$hatwide", 0x0010ff62 }, /* PRIVATE SPACE WIDE CIRCUMFLEX MATH ACCENT */
	{ "$hatwider", 0x0010ff63 }, /* PRIVATE SPACE WIDER CIRCUMFLEX MATH ACCENT */
	{ "$hatwidest", 0x0010ff64 }, /* PRIVATE SPACE WIDEST CIRCUMFLEX MATH ACCENT */
	{ "$hatxwide", 0x0010ffef }, /* PRIVATE SPACE EXTREMELY WIDE CIRCUMFLEX MATH ACCENT */
    	{ "$heartwhite", 0x2661 }, /* WHITE HEART SUIT */
	{ "$hmathbb", 0x0001d559 }, /* MATHEMATICAL DOUBLE-STRUCK SMALL H */
	{ "$hmathfrak", 0x0001d525 }, /* MATHEMATICAL FRAKTUR SMALL H */
        { "$hyphen" , 0x002D }, /* HYPHEN-MINUS; distinguish Adobe duplicates */
        { "$hyphensoft" , 0x00AD }, /* SOFT HYPHEN; distinguish Adobe duplicates */
	{ "$imathbb", 0x0001d55a }, /* MATHEMATICAL DOUBLE-STRUCK SMALL I */
	{ "$imathfrak", 0x0001d526 }, /* MATHEMATICAL FRAKTUR SMALL I */
        { "$increment" , 0x2206 }, /* INCREMENT; distinguish Adobe duplicates */
        { "$information" , 0x2139 }, /* INFORMATION SOURCE */
	{ "$integraldisplay", 0x0010ff5a }, /* PRIVATE SPACE DISPLAY MATH INTEGRAL */
	{ "$integraltext", 0x0010ff52 }, /* PRIVATE SPACE INLINE MATH INTEGRAL */
	{ "$intercalate", 0x22ba }, /* INTERCALATE */
	{ "$interrobang", 0x203d }, /* INTERROBANG */
    	{ "$intersectiondbl", 0x22d2 }, /* DOUBLE INTERSECTION */
	{ "$intersectiondisplay", 0x0010ff5c }, /* PRIVATE SPACE DISPLAY MATH INTERSECTION */
    	{ "$intersectionsq", 0x2293 }, /* SQUARE CAP */
	{ "$intersectiontext", 0x0010ff54 }, /* PRIVATE SPACE INLINE MATH INTERSECTION */
	{ "$jmathbb", 0x0001d55b }, /* MATHEMATICAL DOUBLE-STRUCK SMALL J */
	{ "$jmathfrak", 0x0001d527 }, /* MATHEMATICAL FRAKTUR SMALL J */
    	{ "$jupiter", 0x2643 }, /* JUPITER */
	{ "$kappa1", 0x03f0 }, /* GREEK KAPPA SYMBOL */
        { "$keyboard" , 0x2328 }, /* KEYBOARD */
	{ "$kmathbb", 0x0001d55c }, /* MATHEMATICAL DOUBLE-STRUCK SMALL K */
	{ "$kmathfrak", 0x0001d528 }, /* MATHEMATICAL FRAKTUR SMALL K */
    	{ "$latincross", 0x271d }, /* LATIN CROSS */
    	{ "$latticetop", 0x22a4 }, /* DOWN TACK */
	{ "$leftthreetimes", 0x22cb }, /* LEFT SEMIDIRECT PRODUCT  */
	{ "$lefttimes", 0x22c9 }, /* LEFT NORMAL FACTOR SEMIDIRECT PRODUCT */
    	{ "$lefttorque", 0x2939 }, /* LEFT-SIDE ARC ANTICLOCKWISE ARROW */
    	{ "$leo", 0x264c }, /* LEO */
	{ "$lessapprox", 0x2a85 }, /* LESS-THAN OR APPROXIMATE */
	{ "$lessdot", 0x22d6 }, /* LESS-THAN WITH DOT */
	{ "$lessequal2", 0x2266 }, /* LESS-THAN OVER EQUAl TO */
	{ "$lessequalgreater", 0x22da }, /* LESS-THAN EQUAL TO OR GREATER-THAN */
	{ "$lessequalgreaterstacked", 0x2a8b }, /* LESS-THAN ABOVE DOUBLE-LINE EQUAL ABOVE GREATER-THAN */
	{ "$lessequalslanted", 0x2a7d }, /* LESS-THAN OR SLANTED EQUAL TO */
	{ "$lessgreater", 0x2276 }, /* LESS-THAN OR GREATER-THAN */
    	{ "$lessmuch", 0x226a }, /* MUCH LESS-THAN */
	{ "$lessnotapprox", 0x2a89 }, /* LESS-THAN AND NOT APPROXIMATE */
	{ "$lessnotequal", 0x2268 }, /* LESS-THAN BUT NOT EQUAL TO */
	{ "$lessnotequal1", 0x2a87 }, /* LESS-THAN AND SINGLE-LINE NOT EQUAL TO */
	{ "$lessnotsimilar", 0x22e6 }, /* LESS-THAN BUT NOT EQUIVALENT TO */
	{ "$lesssimilar", 0x2272 }, /* LESS-THAN OR EQUIVALENT TO */
    	{ "$lessverymuch", 0x22d8 }, /* VERY MUCH LESS-THAN */
    	{ "$libra", 0x264e }, /* LIBRA */
	{ "$llcorner", 0x231e }, /* BOTTOM LEFT CORNER */
	{ "$lmathbb", 0x0001d55d }, /* MATHEMATICAL DOUBLE-STRUCK SMALL L */
	{ "$lmathfrak", 0x0001d529 }, /* MATHEMATICAL FRAKTUR SMALL L */
	{ "$logicalanddisplay", 0x0010ff5e }, /* PRIVATE SPACE DISPLAY MATH LOGICAL AND */
	{ "$logicalandtext", 0x0010ff56 }, /* PRIVATE SPACE INLINE MATH LOGICAL AND */
	{ "$logicalordisplay", 0x0010ff5f }, /* PRIVATE SPACE DISPLAY MATH LOGICAL OR */
	{ "$logicalortext", 0x0010ff57 }, /* PRIVATE SPACE INLINE MATH LOGICAL OR */
    	{ "$longst", 0xfb05 }, /* LATIN SMALL LIGATURE LONG S T */
	{ "$looparrowleft", 0x21ab }, /* LEFTWARDS ARROW WITH LOOP */
	{ "$looparrowright", 0x21ac }, /* RIGHTWARDS ARROW WITH LOOP */
	{ "$lrcorner", 0x231f }, /* BOTTOM RIGHT CORNER */
    	{ "$lscript", 0x2113 }, /* SCRIPT SMALL L; Adobe has this as "afii61289" */
        { "$macron", 0x00AF }, /* MACRON; distinguish Adobe duplicates */
    	{ "$macroncomb", 0x0304 }, /* COMBINING MACRON */
    	{ "$macronmodifier", 0x02c9 }, /* MODIFIER LETTER MACRON */
    	{ "$maltesecross", 0x2720 }, /* MALTESE CROSS */
    	{ "$measuredangle", 0x2221 }, /* MEASURED ANGLE */
    	{ "$mercury", 0x263f }, /* MERCURY */
        { "$micro", 0x00B5 }, /* MICRO SIGN; distinguish Adobe duplicates */
    	{ "$minusplus", 0x2213 }, /* MINUS-OR-PLUS SIGN */
        { "$minutereversed", 0x2035 }, /* REVERSED PRIME */
	{ "$mmathbb", 0x0001d55e }, /* MATHEMATICAL DOUBLE-STRUCK SMALL M */
	{ "$mmathfrak", 0x0001d52a }, /* MATHEMATICAL FRAKTUR SMALL M */
    	{ "$moonfirstquarter", 0x263d }, /* FIRST QUARTER MOON */
        { "$mu", 0x03BC }, /* GREEK SMALL LETTER MU; distinguish Adobe duplicates */
	{ "$multimap", 0x22b8 }, /* MULTIMAP */
    	{ "$naira", 0x20a6 }, /* NAIRA SIGN */
    	{ "$natural", 0x266e }, /* MUSIC NATURAL SIGN */
    	{ "$negationslash", 0x0010fffc }, /* PRIVATE SPACE NEGATION SLASH */
    	{ "$neptune", 0x2646 }, /* NEPTUNE */
        { "$ng", 0x0010fffe }, /* PRIVATE SPACE LIGATURE SMALL NG */
	{ "$ninemathbb", 0x0001de1 }, /* MATHEMATICAL DOUBLE-STRUCK DIGIT NINE */
	{ "$nmathbb", 0x0001d55f }, /* MATHEMATICAL DOUBLE-STRUCK SMALL N */
	{ "$nmathfrak", 0x0001d52b }, /* MATHEMATICAL FRAKTUR SMALL N */
        { "$normalin", 0x22b2 }, /* NORMAL SUBGROUP OF */
        { "$normalizes", 0x22b3 }, /* CONTAINS AS NORMAL SUBGROUP */
	{ "$notarrowboth", 0x21ae }, /* LEFT RIGHT ARROW WITH STROKE */
	{ "$notarrowdblboth", 0x21ce }, /* LEFT RIGHT DOUBLE ARROW WITH STROKE */
	{ "$notarrowdblleft", 0x21cd }, /* LEFTWARDS DOUBLE ARROW WITH STROKE */
	{ "$notarrowdblright", 0x21cf }, /* RIGHTWARDS DOUBLE ARROW WITH STROKE */
	{ "$notarrowleft", 0x219a }, /* LEFTWARDS ARROW WITH STROKE */
	{ "$notarrowright", 0x219b }, /* RIGHTWARDS ARROW WITH STROKE */
	{ "$notcongruent", 0x2247 }, /* NEITHER APPROXIMATELY NOR ACTUALLY EQUAL TO */
    	{ "$notequivalent", 0x2262 }, /* NOT IDENTICAL TO */
	{ "$notexistential", 0x2204 }, /* THERE DOES NOT EXIST */
	{ "$notfollowsequal", 0x22e1 }, /* DOES NOT SUCCEED OR EQUAL */
	{ "$notforcesdoublehbar", 0x22af }, /* NEGATED DOUBLE VERTICAL BAR DOUBLE RIGHT TURNSTILE */
	{ "$notgreater", 0x226f }, /* NOT GREATER-THAN */
	{ "$notgreaterequal", 0x2271 }, /* NEITHER GREATER-THAN NOR EQUAL TO */
	{ "$notgreaterequal2", 0x0010fff3 }, /* PRIVATE SPACE NEGATED GREATER-THAN OVER EQUAL TO */
	{ "$notgreaterequalslanted", 0x0010fff5 }, /* PRIVATE SPACE NEGATED GREATER-THAN OR SLANTED EQUAL TO */
	{ "$notless", 0x226e }, /* NOT LESS-THAN */
	{ "$notlessequal", 0x2270 }, /* NEITHER LESS-THAN NOR EQUAL TO */
	{ "$notlessequal2", 0x0010fff4 }, /* PRIVATE SPACE NEGATED LESS-THAN OVER EQUAL TO */
	{ "$notlessequalslanted", 0x0010fff6 }, /* PRIVATE SPACE NEGATED LESS-THAN OR SLANTED EQUAL TO */
	{ "$notnormalin", 0x22ea }, /* NOT NORMAL SUBGROUP OF */
	{ "$notnormalizes", 0x22eb }, /* DOES NOT CONTAIN AS NORMAL SUBGROUP */
	{ "$notparallel", 0x2226 }, /* NOT PARALLEL TO */
	{ "$notprecedesequal", 0x22e0 }, /* DOES NOT PRECEDE OR EQUAL */
	{ "$notreflexnormalin", 0x22ec }, /* NOT NORMAL SUBGROUP OF OR EQUAL TO */
	{ "$notreflexnormalizes", 0x22ed }, /* DOES NOT CONTAIN AS NORMAL SUBGROUP OR EQUAL */
	{ "$notsimilar", 0x2241 }, /* NOT TILDE */
	{ "$notsubsetequal", 0x2288 }, /* NEITHER A SUBSET OF NOR EQUAL TO */
	{ "$notsubsetequal2", 0x0010fff2 }, /* PRIVATE SPACE NEGATED SUBSET OF ABOVE EQUALS SIGN */
	{ "$notsupersetequal", 0x2289 }, /* NEITHER A SUPERSET OF NOR EQUAL TO */
	{ "$notsupersetequal2", 0x0010fff1 }, /* PRIVATE SPACE NEGATED SUPERSET OF ABOVE EQUALS SIGN */
	{ "$nottrue", 0x22ad }, /* NOT TRUE */
        { "$numero", 0x2116 }, /* NUMERO SIGN; adobe has this as "afii61352" */
    	{ "$ogonekcomb", 0x0328 }, /* COMBINING OGONEK */
	{ "$omathbb", 0x0001d560 }, /* MATHEMATICAL DOUBLE-STRUCK SMALL O */
	{ "$omathfrak", 0x0001d52c }, /* MATHEMATICAL FRAKTUR SMALL O */
	{ "$onemathbb", 0x0001d7d9 }, /* MATHEMATICAL DOUBLE-STRUCK DIGIT ONE */
    	{ "$overlinecomb", 0x0305 }, /* COMBINING OVERLINE */
    	{ "$parallel", 0x2225 }, /* PARALLEL TO */
	{ "$parenleftBig", 0x0010ff10 }, /* PRIVATE SPACE VERY LARGE LEFT PARENTHESIS */
	{ "$parenleftBigg", 0x0010ff20 }, /* PRIVATE SPACE VERY HUGE LEFT PARENTHESIS */
	{ "$parenleftbig", 0x0010ff00 }, /* PRIVATE SPACE LARGE LEFT PARENTHESIS */
	{ "$parenleftbigg", 0x0010ff12 }, /* PRIVATE SPACE HUGE LEFT PARENTHESIS */
    	{ "$parenleftbt", 0x239d }, /* LEFT PARENTHESIS LOWER HOOK; not Adobe's private space version */
    	{ "$parenleftex", 0x239c }, /* LEFT PARENTHESIS EXTENSION; not Adobe's private space version */
    	{ "$parenlefttp", 0x239b }, /* LEFT PARENTHESIS UPPER HOOK; not Adobe's private space version */
    	{ "$parenleftwhite", 0x2985 }, /* LEFT WHITE PARENTHESIS */
	{ "$parenrightBig", 0x0010ff11 }, /* PRIVATE SPACE VERY LARGE RIGHT PARENTHESIS */
	{ "$parenrightBigg", 0x0010ff21 }, /* PRIVATE SPACE VERY HUGE RIGHT PARENTHESIS */
	{ "$parenrightbig", 0x0010ff01 }, /* PRIVATE SPACE LARGE RIGHT PARENTHESIS */
	{ "$parenrightbigg", 0x0010ff13 }, /* PRIVATE SPACE HUGE RIGHT PARENTHESIS */
    	{ "$parenrightbt", 0x23a0 }, /* RIGHT PARENTHESIS LOWER HOOK; not Adobe's private space version */
    	{ "$parenrightex", 0x239f }, /* RIGHT PARENTHESIS EXTENSION; not Adobe's private space version */
    	{ "$parenrighttp", 0x239e }, /* RIGHT PARENTHESIS UPPER HOOK; not Adobe's private space version */
    	{ "$parenrightwhite", 0x2986 }, /* RIGHT WHITE PARENTHESIS */
        { "$periodcentered" , 0x00B7 }, /* MIDDLE DOT; distinguish Adobe duplicates */
        { "$pertenthousand" , 0x2031 }, /* PER TEN THOUSAND SIGN */
        { "$peso", 0x20b1 }, /* PESO SIGN */
    	{ "$pfennig", 0x20b0 }, /* GERMAN PENNY SIGN */
        { "$phonorecord", 0x2117 }, /* SOUND RECORDING COPYRIGHT */
        { "$pi1", 0x03D6 }, /* GREEK PI SYMBOL; Adobe has this as "omega1" which is too confusing */
    	{ "$pisces", 0x2653 }, /* PISCES */
    	{ "$pitchfork", 0x22d4 }, /* PITCHFORK */
	{ "$planckhbar", 0x210f }, /* PLANCK CONSTANT OVER TWO PI */
    	{ "$pluto", 0x2647 }, /* PLUTO */
	{ "$pmathbb", 0x0001d561 }, /* MATHEMATICAL DOUBLE-STRUCK SMALL P */
	{ "$pmathfrak", 0x0001d52d }, /* MATHEMATICAL FRAKTUR SMALL P */
    	{ "$polishstroke", 0x0010fff8 }, /* PRIVATE SPACE SHORT ASCENDING STROKE */
    	{ "$polishstrokecomb", 0x0010fff7 }, /* PRIVATE SPACE COMBINING SHORT ASCENDING STROKE OVERLAY */
        { "$pounds", 0x2114 }, /* L B BAR SYMBOL */
    	{ "$precedes", 0x227a }, /* PRECEDES */
	{ "$precedesapprox", 0x2ab7 }, /* PRECEDES ABOVE ALMOST EQUAL TO */
    	{ "$precedesequal", 0x227c }, /* PRECEDES OR EQUAL TO */
	{ "$precedesnot", 0x2280 }, /* DOES NOT PRECEDE */
	{ "$precedesnotapprox", 0x2ab9 }, /* PRECEDES ABOVE NOT ALMOST EQUAL TO */
	{ "$precedesnotequal", 0x2ab5 }, /* PRECEDES ABOVE NOT EQUAL TO */
	{ "$precedesnotsimilar", 0x22e8 }, /* PRECEDES BUT NOT EQUIVALENT TO */
	{ "$precedessimilar", 0x227e }, /* PRECEDES OR EQUIVALENT TO */
	{ "$productdisplay", 0x0010ff59 }, /* PRIVATE SPACE DISPLAY MATH N-ARY PRODUCT */
	{ "$producttext", 0x0010ff51 }, /* PRIVATE SPACE INLINE MATH N-ARY PRODUCT */
	{ "$provesnot", 0x22ac }, /* DOES NOT PROVE */
	{ "$qmathbb", 0x0001d562 }, /* MATHEMATICAL DOUBLE-STRUCK SMALL Q */
	{ "$qmathfrak", 0x0001d52e }, /* MATHEMATICAL FRAKTUR SMALL Q */
	{ "$quillbracketleft", 0x2045 }, /* LEFT SQUARE BRACKET WITH QUILL */
	{ "$quillbracketright", 0x2046 }, /* RIGHT SQUARE BRACKET WITH QUILL */
    	{ "$quotedblreversed", 0x201f }, /* DOUBLE HIGH-REVERSED-9 QUOTATION MARK */
	{ "$radicalBig", 0x0010ff71 }, /* PRIVATE SPACE VERY LARGE SQUARE ROOT */
	{ "$radicalBigg", 0x0010ff73 }, /* PRIVATE SPACE VERY HUGE SQUARE ROOT */
	{ "$radicalbig", 0x0010ff70 }, /* PRIVATE SPACE LARGE SQUARE ROOT */
	{ "$radicalbigg", 0x0010ff72 }, /* PRIVATE SPACE HUGE SQUARE ROOT */
	{ "$radicalbt", 0x0010ff74 }, /* PRIVATE SPACE SQUARE ROOT BOTTOM */
	{ "$radicaltp", 0x0010ff76 }, /* PRIVATE SPACE SQUARE ROOT TOP */
	{ "$radicalvertex", 0x0010ff75 }, /* PRIVATE SPACE SQUARE ROOT VERTICAL EXTENDER */
	{ "$radioactive", 0x2622 }, /* RADIOACTIVE SIGN */
        { "$referencemark", 0x203b }, /* REFERENCE MARK */
        { "$reflexnormalin", 0x22b4 }, /* NORMAL SUBGROUP OF OR EQUAL TO */
        { "$reflexnormalizes", 0x22b5 }, /* CONTAINS AS NORMAL SUBGROUP OR EQUAL TO */
    	{ "$reflexsubsetsq", 0x2291 }, /* SQUARE IMAGE OF OR EQUAL TO */
    	{ "$reflexsupersetsq", 0x2292 }, /* SQUARE ORIGINAL OF OR EQUAL TO */
    	{ "$rho1", 0x03f1 }, /* GREEK RHO SYMBOL */
	{ "$rightthreetimes", 0x22cc }, /* RIGHT SEMIDIRECT PRODUCT */
	{ "$righttimes", 0x22ca }, /* RIGHT NORMAL FACTOR SEMIDIRECT PRODUCT */
    	{ "$righttorque", 0x2938 }, /* RIGHT-SIDE ARC CLOCKWISE ARROW */
    	{ "$ringcomb", 0x030a }, /* COMBINING RING ABOVE */
    	{ "$ringequal", 0x2257 }, /* RING EQUAL TO */
    	{ "$ringinequal", 0x2256 }, /* RING IN EQUAL TO */
	{ "$risingdotsequal", 0x2253 }, /* IMAGE OF OR APPROXIMATELY EQUAL TO */
	{ "$rmathbb", 0x0001d563 }, /* MATHEMATICAL DOUBLE-STRUCK SMALL R */
	{ "$rmathfrak", 0x0001d52f }, /* MATHEMATICAL FRAKTUR SMALL R */
    	{ "$sagittarius", 0x2650 }, /* SAGITTARIUS */
    	{ "$saturn", 0x2644 }, /* SATURN */
        { "$scedilla" , 0x015F }, /* LATIN SMALL LETTER S WITH CEDILLA; distinguish Adobe duplicates */
    	{ "$scissors", 0x2702 }, /* BLACK SCISSORS */
    	{ "$scissorsupperblade", 0x2701 }, /* UPPER BLADE SCISSORS */
    	{ "$scorpius", 0x264f }, /* SCORPIUS */
        { "$secondreversed", 0x2036 }, /* REVERSED DOUBLE PRIME */
    	{ "$servicemark", 0x2120 }, /* SERVICE MARK */
	{ "$setminus", 0x2216 }, /* SET MINUS */
	{ "$sevenmathbb", 0x0001d7df }, /* MATHEMATICAL DOUBLE-STRUCK DIGIT SEVEN */
    	{ "$sharp", 0x266f }, /* MUSIC SHARP SIGN */
    	{ "$similarequal", 0x2243 }, /* ASYMPTOTICALLY EQUAL TO */
	{ "$similarequalreversed", 0x22cd }, /* REVERSED TILDE EQUALS */
	{ "$similarreversed", 0x223d }, /* REVERSED TILDE */
	{ "$sixmathbb", 0x0001d7de }, /* MATHEMATICAL DOUBLE-STRUCK DIGIT SIX */
	{ "$slashBig", 0x0010ff2e }, /* PRIVATE SPACE VERY LARGE SOLIDUS */
	{ "$slashBigg", 0x0010ff2c }, /* PRIVATE SPACE VERY HUGE SOLIDUS */
	{ "$slashbig", 0x0010ff0e }, /* PRIVATE SPACE LARGE SOLIDUS */
	{ "$slashbigg", 0x0010ff1e }, /* PRIVATE SPACE HUGE SOLIDUS */
    	{ "$slashlongcomb", 0x0338 }, /* COMBINING LONG SOLIDUS OVERLAY */
	{ "$smathbb", 0x0001d564 }, /* MATHEMATICAL DOUBLE-STRUCK SMALL S */
	{ "$smathfrak", 0x0001d530 }, /* MATHEMATICAL FRAKTUR SMALL S */
    	{ "$smile", 0x2323 }, /* SMILE */
    	{ "$snowflake", 0x2744 }, /* SNOWFLAKE */
        { "$space" , 0x0020 }, /* SPACE; distinguish Adobe duplicates */
        { "$spacenobreak" , 0x00A0 }, /* NO-BREAK SPACE; distinguish Adobe duplicates */
        { "$spacesymbol", 0x2420 }, /* SYMBOL FOR SPACE */
    	{ "$sphericalangle", 0x2222 }, /* SPHERICAL ANGLE */
    	{ "$squigarrowright", 0x21dd }, /* RIGHTWARDS SQUIGGLE ARROW */
    	{ "$st", 0xfb06 }, /* LATIN SMALL LIGATURE ST */
    	{ "$star", 0x2605 }, /* BLACK STAR */
    	{ "$starmath", 0x22c6 }, /* STAR OPERATOR */
    	{ "$subsetdbl", 0x22d0 }, /* DOUBLE SUBSET */
	{ "$subsetequal2", 0x2ac5 }, /* SUBSET OF ABOVE EQUALS SIGN */
	{ "$subsetnotequal", 0x228a }, /* SUBSET OF WITH NOT EQUAL TO */
	{ "$subsetnotequal2", 0x2acb }, /* SUBSET OF ABOVE NOT EQUAL TO */
    	{ "$subsetsq", 0x228f }, /* SQUARE IMAGE OF */
	{ "$summationdisplay", 0x0010ff58 }, /* PRIVATE SPACE DISPLAY MATH N-ARY SUMMATION */
	{ "$summationtext", 0x0010ff50 }, /* PRIVATE SPACE INLINE MATH N-ARY SUMMATION */
    	{ "$sun", 0x2609 }, /* SUN */
    	{ "$supersetdbl", 0x22d1 }, /* DOUBLE SUPERSET */
	{ "$supersetequal2", 0x2ac6 }, /* SUPERSET OF ABOVE EQUALS SIGN */
	{ "$supersetnotequal", 0x228b }, /* SUPERSET OF WITH NOT EQUAL TO */
	{ "$supersetnotequal2", 0x2acc }, /* SUPERSET OF ABOVE NOT EQUAL TO */
    	{ "$supersetsq", 0x2290 }, /* SQUARE ORIGINAL OF */
	{ "$tailarrowleft", 0x21a2 }, /* LEFTWARDS ARROW WITH TAIL */
	{ "$tailarrowright", 0x21a3 }, /* RIGHTWARDS ARROW WITH TAIL */
    	{ "$taurus", 0x2649 }, /* TAURUS */
        { "$tcedilla", 0x0163 }, /* LATIN SMALL LETTER T WITH CEDILLA; distinguish Adobe duplicates */
        { "$tcommaaccent", 0x021B }, /* LATIN SMALL LETTER T WITH COMMA BELOW; distinguish Adobe duplicates */
    	{ "$telephone", 0x260e }, /* BLACK TELEPHONE */
	{ "$threemathbb", 0x0001d7db }, /* MATHEMATICAL DOUBLE-STRUCK DIGIT THREE */
    	{ "$tie", 0x2040 }, /* CHARACTER TIE */
	{ "$tildewide", 0x0010ff65 }, /* PRIVATE SPACE WIDE TILDE MATH ACCENT */
	{ "$tildewider", 0x0010ff66 }, /* PRIVATE SPACE WIDER TILDE MATH ACCENT */
	{ "$tildewidest", 0x0010ff67 }, /* PRIVATE SPACE WIDEST TILDE MATH ACCENT */
	{ "$tildexwide", 0x0010fff0 }, /* PRIVATE SPACE EXTREMELY WIDE TILDE MATH ACCENT */
	{ "$tmathbb", 0x0001d565 }, /* MATHEMATICAL DOUBLE-STRUCK SMALL T */
	{ "$tmathfrak", 0x0001d531 }, /* MATHEMATICAL FRAKTUR SMALL T */
    	{ "$triagwhitedn", 0x25bd }, /* WHITE DOWN-POINTING TRIANGLE */
    	{ "$triagwhiteup", 0x25b3 }, /* WHITE UP-POINTING TRIANGLE */
	{ "$triangleleft", 0x25c0 }, /* BLACK LEFT-POINTING TRIANGLE */
	{ "$triangleright", 0x25b6 }, /* BLACK RIGHT-POINTING TRIANGLE */
	{ "$true", 0x22a8 }, /* TRUE */
    	{ "$turnstileleft", 0x22a2 }, /* RIGHT TACK */
    	{ "$turnstileright", 0x22a3 }, /* LEFT TACK */
	{ "$twoheadarrowleft", 0x219e }, /* LEFTWARDS TWO HEADED ARROW */
	{ "$twoheadarrowright", 0x21a0 }, /* RIGHTWARDS TWO HEADED ARROW */
	{ "$twomathbb", 0x0001d7da }, /* MATHEMATICAL DOUBLE-STRUCK DIGIT TWO */
	{ "$ulcorner", 0x231c }, /* TOP LEFT CORNER */
	{ "$umathbb", 0x0001d566 }, /* MATHEMATICAL DOUBLE-STRUCK SMALL U */
	{ "$umathfrak", 0x0001d532 }, /* MATHEMATICAL FRAKTUR SMALL U */
    	{ "$uniondbl", 0x22d3 }, /* DOUBLE UNION */
	{ "$uniondisplay", 0x0010ff5b }, /* PRIVATE SPACE DISPLAY MATH UNION */
    	{ "$unionmulti", 0x228e }, /* MULTISET UNION */
	{ "$unionmultidisplay", 0x0010ff5d }, /* PRIVATE SPACE DISPLAY MATH MULTISET UNION */
	{ "$unionmultitext", 0x0010ff55 }, /* PRIVATE SPACE INLINE MATH MULTISET UNION */
    	{ "$unionsq", 0x2294 }, /* SQUARE CUP */
	{ "$unionsqdisplay", 0x0010ff47 }, /* PRIVATE SPACE DISPLAY MATH SQUARE CUP */
	{ "$unionsqtext", 0x0010ff46 }, /* PRIVATE SPACE INLINE MATH SQUARE CUP */
	{ "$uniontext", 0x0010ff53 }, /* PRIVATE SPACE INLINE MATH UNION */
    	{ "$uranus", 0x2645 }, /* URANUS */
	{ "$urcorner", 0x231d }, /* TOP RIGHT CORNER */
    	{ "$vector", 0x0010fffb }, /* PRIVATE SPACE SUPERSCRIPT RIGHTWARDS ARROW */
    	{ "$vectorcomb", 0x20d7 }, /* COMBINING RIGHT ARROW ABOVE */
	{ "$veebar", 0x22bb }, /* XOR */
	{ "$vextenddouble", 0x0010ff0d }, /* PRIVATE SPACE DOUBLE VERTICAL LINE EXTENDER */
	{ "$vextendsingle", 0x0010ff0c }, /* PRIVATE SPACE VERTICAL LINE EXTENDER */
    	{ "$virgo", 0x264d }, /* VIRGO */
    	{ "$visualspace", 0x2423 }, /* OPEN BOX */
	{ "$vmathbb", 0x0001d567 }, /* MATHEMATICAL DOUBLE-STRUCK SMALL V */
	{ "$vmathfrak", 0x0001d533 }, /* MATHEMATICAL FRAKTUR SMALL V */
    	{ "$watch", 0x231a }, /* WATCH */
	{ "$wmathbb", 0x0001d568 }, /* MATHEMATICAL DOUBLE-STRUCK SMALL W */
	{ "$wmathfrak", 0x0001d534 }, /* MATHEMATICAL FRAKTUR SMALL W */
    	{ "$won", 0x20a9 }, /* WON SIGN */
    	{ "$wreathproduct", 0x2240 }, /* WREATH PRODUCT */
    	{ "$writinghand", 0x270d }, /* WRITING HAND */
	{ "$xmathbb", 0x0001d569 }, /* MATHEMATICAL DOUBLE-STRUCK SMALL X */
	{ "$xmathfrak", 0x0001d535 }, /* MATHEMATICAL FRAKTUR SMALL X */
    	{ "$yinyang", 0x262f }, /* YIN YANG */
	{ "$ymathbb", 0x0001d56a }, /* MATHEMATICAL DOUBLE-STRUCK SMALL Y */
	{ "$ymathfrak", 0x0001d536 }, /* MATHEMATICAL FRAKTUR SMALL Y */
	{ "$zeromathbb", 0x0001d7d8 }, /* MATHEMATICAL DOUBLE-STRUCK DIGIT ZERO */
    	{ "$zigzagarrowdown", 0x21af }, /* DOWNWARDS ZIGZAG ARROW */
	{ "$zmathbb", 0x0001d56b }, /* MATHEMATICAL DOUBLE-STRUCK SMALL Z */
	{ "$zmathfrak", 0x0001d537 }, /* MATHEMATICAL FRAKTUR SMALL Z */
        { ".notavail", 0xfffd }, /* REPLACEMENT CHARACTER */
        { ".notdef", 0x003f }, /* QUESTION MARK */
        { "A", 0x0041 }, /* LATIN CAPITAL LETTER A */
        { "AE", 0x00C6 }, /* LATIN CAPITAL LETTER AE */
        { "AEacute", 0x01FC }, /* LATIN CAPITAL LETTER AE WITH ACUTE */
        { "AEsmall", 0xF7E6 }, /* LATIN SMALL CAPITAL LETTER AE */
        { "Aacute", 0x00C1 }, /* LATIN CAPITAL LETTER A WITH ACUTE */
        { "Aacutesmall", 0xF7E1 }, /* LATIN SMALL CAPITAL LETTER A WITH ACUTE */
        { "Abreve", 0x0102 }, /* LATIN CAPITAL LETTER A WITH BREVE */
        { "Acircumflex", 0x00C2 }, /* LATIN CAPITAL LETTER A WITH CIRCUMFLEX */
        { "Acircumflexsmall", 0xF7E2 }, /* LATIN SMALL CAPITAL LETTER A WITH CIRCUMFLEX */
        { "Acute", 0xF6C9 }, /* CAPITAL ACUTE ACCENT */
        { "Acutesmall", 0xF7B4 }, /* SMALL CAPITAL ACUTE ACCENT */
        { "Adieresis", 0x00C4 }, /* LATIN CAPITAL LETTER A WITH DIAERESIS */
        { "Adieresissmall", 0xF7E4 }, /* LATIN SMALL CAPITAL LETTER A WITH DIAERESIS */
        { "Agrave", 0x00C0 }, /* LATIN CAPITAL LETTER A WITH GRAVE */
        { "Agravesmall", 0xF7E0 }, /* LATIN SMALL CAPITAL LETTER A WITH GRAVE */
        { "Alpha", 0x0391 }, /* GREEK CAPITAL LETTER ALPHA */
        { "Alphatonos", 0x0386 }, /* GREEK CAPITAL LETTER ALPHA WITH TONOS */
        { "Amacron", 0x0100 }, /* LATIN CAPITAL LETTER A WITH MACRON */
        { "Aogonek", 0x0104 }, /* LATIN CAPITAL LETTER A WITH OGONEK */
        { "Aring", 0x00C5 }, /* LATIN CAPITAL LETTER A WITH RING ABOVE */
        { "Aringacute", 0x01FA }, /* LATIN CAPITAL LETTER A WITH RING ABOVE AND ACUTE */
        { "Aringsmall", 0xF7E5 }, /* LATIN SMALL CAPITAL LETTER A WITH RING ABOVE */
        { "Asmall", 0xF761 }, /* LATIN SMALL CAPITAL LETTER A */
        { "Atilde", 0x00C3 }, /* LATIN CAPITAL LETTER A WITH TILDE */
        { "Atildesmall", 0xF7E3 }, /* LATIN SMALL CAPITAL LETTER A WITH TILDE */
        { "B", 0x0042 }, /* LATIN CAPITAL LETTER B */
        { "Beta", 0x0392 }, /* GREEK CAPITAL LETTER BETA */
        { "Brevesmall", 0xF6F4 }, /* SMALL CAPITAL BREVE */
        { "Bsmall", 0xF762 }, /* LATIN SMALL CAPITAL LETTER B */
        { "C", 0x0043 }, /* LATIN CAPITAL LETTER C */
        { "Cacute", 0x0106 }, /* LATIN CAPITAL LETTER C WITH ACUTE */
        { "Caron", 0xF6CA }, /* CAPITAL CARON */
        { "Caronsmall", 0xF6F5 }, /* SMALL CAPITAL CARON */
        { "Ccaron", 0x010C }, /* LATIN CAPITAL LETTER C WITH CARON */
        { "Ccedilla", 0x00C7 }, /* LATIN CAPITAL LETTER C WITH CEDILLA */
        { "Ccedillasmall", 0xF7E7 }, /* LATIN SMALL CAPITAL LETTER C WITH CEDILLA */
        { "Ccircumflex", 0x0108 }, /* LATIN CAPITAL LETTER C WITH CIRCUMFLEX */
        { "Cdotaccent", 0x010A }, /* LATIN CAPITAL LETTER C WITH DOT ABOVE */
        { "Cedillasmall", 0xF7B8 }, /* SMALL CAPITAL CEDILLA */
        { "Chi", 0x03A7 }, /* GREEK CAPITAL LETTER CHI */
        { "Circumflexsmall", 0xF6F6 }, /* SMALL CAPITAL MODIFIER LETTER CIRCUMFLEX ACCENT */
        { "Csmall", 0xF763 }, /* LATIN SMALL CAPITAL LETTER C */
        { "D", 0x0044 }, /* LATIN CAPITAL LETTER D */
        { "Dcaron", 0x010E }, /* LATIN CAPITAL LETTER D WITH CARON */
        { "Dcroat", 0x0110 }, /* LATIN CAPITAL LETTER D WITH STROKE */
        { "Delta" DUP1, 0x2206 }, /* INCREMENT */
        { "Delta" DUP2, 0x0394 }, /* GREEK CAPITAL LETTER DELTA;Duplicate */
        { "Dieresis", 0xF6CB }, /* CAPITAL DIAERESIS */
        { "DieresisAcute", 0xF6CC }, /* CAPITAL DIAERESIS ACUTE ACCENT */
        { "DieresisGrave", 0xF6CD }, /* CAPITAL DIAERESIS GRAVE ACCENT */
        { "Dieresissmall", 0xF7A8 }, /* SMALL CAPITAL DIAERESIS */
        { "Dotaccentsmall", 0xF6F7 }, /* SMALL CAPITAL DOT ABOVE */
        { "Dsmall", 0xF764 }, /* LATIN SMALL CAPITAL LETTER D */
        { "E", 0x0045 }, /* LATIN CAPITAL LETTER E */
        { "Eacute", 0x00C9 }, /* LATIN CAPITAL LETTER E WITH ACUTE */
        { "Eacutesmall", 0xF7E9 }, /* LATIN SMALL CAPITAL LETTER E WITH ACUTE */
        { "Ebreve", 0x0114 }, /* LATIN CAPITAL LETTER E WITH BREVE */
        { "Ecaron", 0x011A }, /* LATIN CAPITAL LETTER E WITH CARON */
        { "Ecircumflex", 0x00CA }, /* LATIN CAPITAL LETTER E WITH CIRCUMFLEX */
        { "Ecircumflexsmall", 0xF7EA }, /* LATIN SMALL CAPITAL LETTER E WITH CIRCUMFLEX */
        { "Edieresis", 0x00CB }, /* LATIN CAPITAL LETTER E WITH DIAERESIS */
        { "Edieresissmall", 0xF7EB }, /* LATIN SMALL CAPITAL LETTER E WITH DIAERESIS */
        { "Edotaccent", 0x0116 }, /* LATIN CAPITAL LETTER E WITH DOT ABOVE */
        { "Egrave", 0x00C8 }, /* LATIN CAPITAL LETTER E WITH GRAVE */
        { "Egravesmall", 0xF7E8 }, /* LATIN SMALL CAPITAL LETTER E WITH GRAVE */
        { "Emacron", 0x0112 }, /* LATIN CAPITAL LETTER E WITH MACRON */
        { "Eng", 0x014A }, /* LATIN CAPITAL LETTER ENG */
        { "Eogonek", 0x0118 }, /* LATIN CAPITAL LETTER E WITH OGONEK */
        { "Epsilon", 0x0395 }, /* GREEK CAPITAL LETTER EPSILON */
        { "Epsilontonos", 0x0388 }, /* GREEK CAPITAL LETTER EPSILON WITH TONOS */
        { "Esmall", 0xF765 }, /* LATIN SMALL CAPITAL LETTER E */
        { "Eta", 0x0397 }, /* GREEK CAPITAL LETTER ETA */
        { "Etatonos", 0x0389 }, /* GREEK CAPITAL LETTER ETA WITH TONOS */
        { "Eth", 0x00D0 }, /* LATIN CAPITAL LETTER ETH */
        { "Ethsmall", 0xF7F0 }, /* LATIN SMALL CAPITAL LETTER ETH */
        { "Euro", 0x20AC }, /* EURO SIGN */
        { "F", 0x0046 }, /* LATIN CAPITAL LETTER F */
        { "Fsmall", 0xF766 }, /* LATIN SMALL CAPITAL LETTER F */
        { "G", 0x0047 }, /* LATIN CAPITAL LETTER G */
        { "Gamma", 0x0393 }, /* GREEK CAPITAL LETTER GAMMA */
        { "Gbreve", 0x011E }, /* LATIN CAPITAL LETTER G WITH BREVE */
        { "Gcaron", 0x01E6 }, /* LATIN CAPITAL LETTER G WITH CARON */
        { "Gcircumflex", 0x011C }, /* LATIN CAPITAL LETTER G WITH CIRCUMFLEX */
        { "Gcommaaccent", 0x0122 }, /* LATIN CAPITAL LETTER G WITH CEDILLA */
        { "Gdotaccent", 0x0120 }, /* LATIN CAPITAL LETTER G WITH DOT ABOVE */
        { "Grave", 0xF6CE }, /* CAPITAL GRAVE ACCENT */
        { "Gravesmall", 0xF760 }, /* SMALL CAPITAL GRAVE ACCENT */
        { "Gsmall", 0xF767 }, /* LATIN SMALL CAPITAL LETTER G */
        { "H", 0x0048 }, /* LATIN CAPITAL LETTER H */
        { "H18533", 0x25CF }, /* BLACK CIRCLE */
        { "H18543", 0x25AA }, /* BLACK SMALL SQUARE */
        { "H18551", 0x25AB }, /* WHITE SMALL SQUARE */
        { "H22073", 0x25A1 }, /* WHITE SQUARE */
        { "Hbar", 0x0126 }, /* LATIN CAPITAL LETTER H WITH STROKE */
        { "Hcircumflex", 0x0124 }, /* LATIN CAPITAL LETTER H WITH CIRCUMFLEX */
        { "Hsmall", 0xF768 }, /* LATIN SMALL CAPITAL LETTER H */
        { "Hungarumlaut", 0xF6CF }, /* CAPITAL DOUBLE ACUTE ACCENT */
        { "Hungarumlautsmall", 0xF6F8 }, /* SMALL CAPITAL DOUBLE ACUTE ACCENT */
        { "I", 0x0049 }, /* LATIN CAPITAL LETTER I */
        { "IJ", 0x0132 }, /* LATIN CAPITAL LIGATURE IJ */
        { "Iacute", 0x00CD }, /* LATIN CAPITAL LETTER I WITH ACUTE */
        { "Iacutesmall", 0xF7ED }, /* LATIN SMALL CAPITAL LETTER I WITH ACUTE */
        { "Ibreve", 0x012C }, /* LATIN CAPITAL LETTER I WITH BREVE */
        { "Icircumflex", 0x00CE }, /* LATIN CAPITAL LETTER I WITH CIRCUMFLEX */
        { "Icircumflexsmall", 0xF7EE }, /* LATIN SMALL CAPITAL LETTER I WITH CIRCUMFLEX */
        { "Idieresis", 0x00CF }, /* LATIN CAPITAL LETTER I WITH DIAERESIS */
        { "Idieresissmall", 0xF7EF }, /* LATIN SMALL CAPITAL LETTER I WITH DIAERESIS */
        { "Idotaccent", 0x0130 }, /* LATIN CAPITAL LETTER I WITH DOT ABOVE */
        { "Ifraktur", 0x2111 }, /* BLACK-LETTER CAPITAL I */
        { "Igrave", 0x00CC }, /* LATIN CAPITAL LETTER I WITH GRAVE */
        { "Igravesmall", 0xF7EC }, /* LATIN SMALL CAPITAL LETTER I WITH GRAVE */
        { "Imacron", 0x012A }, /* LATIN CAPITAL LETTER I WITH MACRON */
        { "Iogonek", 0x012E }, /* LATIN CAPITAL LETTER I WITH OGONEK */
        { "Iota", 0x0399 }, /* GREEK CAPITAL LETTER IOTA */
        { "Iotadieresis", 0x03AA }, /* GREEK CAPITAL LETTER IOTA WITH DIALYTIKA */
        { "Iotatonos", 0x038A }, /* GREEK CAPITAL LETTER IOTA WITH TONOS */
        { "Ismall", 0xF769 }, /* LATIN SMALL CAPITAL LETTER I */
        { "Itilde", 0x0128 }, /* LATIN CAPITAL LETTER I WITH TILDE */
        { "J", 0x004A }, /* LATIN CAPITAL LETTER J */
        { "Jcircumflex", 0x0134 }, /* LATIN CAPITAL LETTER J WITH CIRCUMFLEX */
        { "Jsmall", 0xF76A }, /* LATIN SMALL CAPITAL LETTER J */
        { "K", 0x004B }, /* LATIN CAPITAL LETTER K */
        { "Kappa", 0x039A }, /* GREEK CAPITAL LETTER KAPPA */
        { "Kcommaaccent", 0x0136 }, /* LATIN CAPITAL LETTER K WITH CEDILLA */
        { "Ksmall", 0xF76B }, /* LATIN SMALL CAPITAL LETTER K */
        { "L", 0x004C }, /* LATIN CAPITAL LETTER L */
        { "LL", 0xF6BF }, /* LATIN CAPITAL LETTER LL */
        { "Lacute", 0x0139 }, /* LATIN CAPITAL LETTER L WITH ACUTE */
        { "Lambda", 0x039B }, /* GREEK CAPITAL LETTER LAMDA */
        { "Lcaron", 0x013D }, /* LATIN CAPITAL LETTER L WITH CARON */
        { "Lcommaaccent", 0x013B }, /* LATIN CAPITAL LETTER L WITH CEDILLA */
        { "Ldot", 0x013F }, /* LATIN CAPITAL LETTER L WITH MIDDLE DOT */
        { "Lslash", 0x0141 }, /* LATIN CAPITAL LETTER L WITH STROKE */
        { "Lslashsmall", 0xF6F9 }, /* LATIN SMALL CAPITAL LETTER L WITH STROKE */
        { "Lsmall", 0xF76C }, /* LATIN SMALL CAPITAL LETTER L */
        { "M", 0x004D }, /* LATIN CAPITAL LETTER M */
        { "Macron", 0xF6D0 }, /* CAPITAL MACRON */
        { "Macronsmall", 0xF7AF }, /* SMALL CAPITAL MACRON */
        { "Msmall", 0xF76D }, /* LATIN SMALL CAPITAL LETTER M */
        { "Mu", 0x039C }, /* GREEK CAPITAL LETTER MU */
        { "N", 0x004E }, /* LATIN CAPITAL LETTER N */
        { "Nacute", 0x0143 }, /* LATIN CAPITAL LETTER N WITH ACUTE */
        { "Ncaron", 0x0147 }, /* LATIN CAPITAL LETTER N WITH CARON */
        { "Ncommaaccent", 0x0145 }, /* LATIN CAPITAL LETTER N WITH CEDILLA */
        { "Nsmall", 0xF76E }, /* LATIN SMALL CAPITAL LETTER N */
        { "Ntilde", 0x00D1 }, /* LATIN CAPITAL LETTER N WITH TILDE */
        { "Ntildesmall", 0xF7F1 }, /* LATIN SMALL CAPITAL LETTER N WITH TILDE */
        { "Nu", 0x039D }, /* GREEK CAPITAL LETTER NU */
        { "O", 0x004F }, /* LATIN CAPITAL LETTER O */
        { "OE", 0x0152 }, /* LATIN CAPITAL LIGATURE OE */
        { "OEsmall", 0xF6FA }, /* LATIN SMALL CAPITAL LIGATURE OE */
        { "Oacute", 0x00D3 }, /* LATIN CAPITAL LETTER O WITH ACUTE */
        { "Oacutesmall", 0xF7F3 }, /* LATIN SMALL CAPITAL LETTER O WITH ACUTE */
        { "Obreve", 0x014E }, /* LATIN CAPITAL LETTER O WITH BREVE */
        { "Ocircumflex", 0x00D4 }, /* LATIN CAPITAL LETTER O WITH CIRCUMFLEX */
        { "Ocircumflexsmall", 0xF7F4 }, /* LATIN SMALL CAPITAL LETTER O WITH CIRCUMFLEX */
        { "Odieresis", 0x00D6 }, /* LATIN CAPITAL LETTER O WITH DIAERESIS */
        { "Odieresissmall", 0xF7F6 }, /* LATIN SMALL CAPITAL LETTER O WITH DIAERESIS */
        { "Ogoneksmall", 0xF6FB }, /* SMALL CAPITAL OGONEK */
        { "Ograve", 0x00D2 }, /* LATIN CAPITAL LETTER O WITH GRAVE */
        { "Ogravesmall", 0xF7F2 }, /* LATIN SMALL CAPITAL LETTER O WITH GRAVE */
        { "Ohorn", 0x01A0 }, /* LATIN CAPITAL LETTER O WITH HORN */
        { "Ohungarumlaut", 0x0150 }, /* LATIN CAPITAL LETTER O WITH DOUBLE ACUTE */
        { "Omacron", 0x014C }, /* LATIN CAPITAL LETTER O WITH MACRON */
        { "Omega" DUP1, 0x2126 }, /* OHM SIGN */
        { "Omega" DUP2, 0x03A9 }, /* GREEK CAPITAL LETTER OMEGA;Duplicate */
        { "Omegatonos", 0x038F }, /* GREEK CAPITAL LETTER OMEGA WITH TONOS */
        { "Omicron", 0x039F }, /* GREEK CAPITAL LETTER OMICRON */
        { "Omicrontonos", 0x038C }, /* GREEK CAPITAL LETTER OMICRON WITH TONOS */
        { "Oslash", 0x00D8 }, /* LATIN CAPITAL LETTER O WITH STROKE */
        { "Oslashacute", 0x01FE }, /* LATIN CAPITAL LETTER O WITH STROKE AND ACUTE */
        { "Oslashsmall", 0xF7F8 }, /* LATIN SMALL CAPITAL LETTER O WITH STROKE */
        { "Osmall", 0xF76F }, /* LATIN SMALL CAPITAL LETTER O */
        { "Otilde", 0x00D5 }, /* LATIN CAPITAL LETTER O WITH TILDE */
        { "Otildesmall", 0xF7F5 }, /* LATIN SMALL CAPITAL LETTER O WITH TILDE */
        { "P", 0x0050 }, /* LATIN CAPITAL LETTER P */
        { "Phi", 0x03A6 }, /* GREEK CAPITAL LETTER PHI */
        { "Pi", 0x03A0 }, /* GREEK CAPITAL LETTER PI */
        { "Psi", 0x03A8 }, /* GREEK CAPITAL LETTER PSI */
        { "Psmall", 0xF770 }, /* LATIN SMALL CAPITAL LETTER P */
        { "Q", 0x0051 }, /* LATIN CAPITAL LETTER Q */
        { "Qsmall", 0xF771 }, /* LATIN SMALL CAPITAL LETTER Q */
        { "R", 0x0052 }, /* LATIN CAPITAL LETTER R */
        { "Racute", 0x0154 }, /* LATIN CAPITAL LETTER R WITH ACUTE */
        { "Rcaron", 0x0158 }, /* LATIN CAPITAL LETTER R WITH CARON */
        { "Rcommaaccent", 0x0156 }, /* LATIN CAPITAL LETTER R WITH CEDILLA */
        { "Rfraktur", 0x211C }, /* BLACK-LETTER CAPITAL R */
        { "Rho", 0x03A1 }, /* GREEK CAPITAL LETTER RHO */
        { "Ringsmall", 0xF6FC }, /* SMALL CAPITAL RING ABOVE */
        { "Rsmall", 0xF772 }, /* LATIN SMALL CAPITAL LETTER R */
        { "S", 0x0053 }, /* LATIN CAPITAL LETTER S */
        { "SF010000", 0x250C }, /* BOX DRAWINGS LIGHT DOWN AND RIGHT */
        { "SF020000", 0x2514 }, /* BOX DRAWINGS LIGHT UP AND RIGHT */
        { "SF030000", 0x2510 }, /* BOX DRAWINGS LIGHT DOWN AND LEFT */
        { "SF040000", 0x2518 }, /* BOX DRAWINGS LIGHT UP AND LEFT */
        { "SF050000", 0x253C }, /* BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL */
        { "SF060000", 0x252C }, /* BOX DRAWINGS LIGHT DOWN AND HORIZONTAL */
        { "SF070000", 0x2534 }, /* BOX DRAWINGS LIGHT UP AND HORIZONTAL */
        { "SF080000", 0x251C }, /* BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
        { "SF090000", 0x2524 }, /* BOX DRAWINGS LIGHT VERTICAL AND LEFT */
        { "SF100000", 0x2500 }, /* BOX DRAWINGS LIGHT HORIZONTAL */
        { "SF110000", 0x2502 }, /* BOX DRAWINGS LIGHT VERTICAL */
        { "SF190000", 0x2561 }, /* BOX DRAWINGS VERTICAL SINGLE AND LEFT DOUBLE */
        { "SF200000", 0x2562 }, /* BOX DRAWINGS VERTICAL DOUBLE AND LEFT SINGLE */
        { "SF210000", 0x2556 }, /* BOX DRAWINGS DOWN DOUBLE AND LEFT SINGLE */
        { "SF220000", 0x2555 }, /* BOX DRAWINGS DOWN SINGLE AND LEFT DOUBLE */
        { "SF230000", 0x2563 }, /* BOX DRAWINGS DOUBLE VERTICAL AND LEFT */
        { "SF240000", 0x2551 }, /* BOX DRAWINGS DOUBLE VERTICAL */
        { "SF250000", 0x2557 }, /* BOX DRAWINGS DOUBLE DOWN AND LEFT */
        { "SF260000", 0x255D }, /* BOX DRAWINGS DOUBLE UP AND LEFT */
        { "SF270000", 0x255C }, /* BOX DRAWINGS UP DOUBLE AND LEFT SINGLE */
        { "SF280000", 0x255B }, /* BOX DRAWINGS UP SINGLE AND LEFT DOUBLE */
        { "SF360000", 0x255E }, /* BOX DRAWINGS VERTICAL SINGLE AND RIGHT DOUBLE */
        { "SF370000", 0x255F }, /* BOX DRAWINGS VERTICAL DOUBLE AND RIGHT SINGLE */
        { "SF380000", 0x255A }, /* BOX DRAWINGS DOUBLE UP AND RIGHT */
        { "SF390000", 0x2554 }, /* BOX DRAWINGS DOUBLE DOWN AND RIGHT */
        { "SF400000", 0x2569 }, /* BOX DRAWINGS DOUBLE UP AND HORIZONTAL */
        { "SF410000", 0x2566 }, /* BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL */
        { "SF420000", 0x2560 }, /* BOX DRAWINGS DOUBLE VERTICAL AND RIGHT */
        { "SF430000", 0x2550 }, /* BOX DRAWINGS DOUBLE HORIZONTAL */
        { "SF440000", 0x256C }, /* BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL */
        { "SF450000", 0x2567 }, /* BOX DRAWINGS UP SINGLE AND HORIZONTAL DOUBLE */
        { "SF460000", 0x2568 }, /* BOX DRAWINGS UP DOUBLE AND HORIZONTAL SINGLE */
        { "SF470000", 0x2564 }, /* BOX DRAWINGS DOWN SINGLE AND HORIZONTAL DOUBLE */
        { "SF480000", 0x2565 }, /* BOX DRAWINGS DOWN DOUBLE AND HORIZONTAL SINGLE */
        { "SF490000", 0x2559 }, /* BOX DRAWINGS UP DOUBLE AND RIGHT SINGLE */
        { "SF500000", 0x2558 }, /* BOX DRAWINGS UP SINGLE AND RIGHT DOUBLE */
        { "SF510000", 0x2552 }, /* BOX DRAWINGS DOWN SINGLE AND RIGHT DOUBLE */
        { "SF520000", 0x2553 }, /* BOX DRAWINGS DOWN DOUBLE AND RIGHT SINGLE */
        { "SF530000", 0x256B }, /* BOX DRAWINGS VERTICAL DOUBLE AND HORIZONTAL SINGLE */
        { "SF540000", 0x256A }, /* BOX DRAWINGS VERTICAL SINGLE AND HORIZONTAL DOUBLE */
        { "Sacute", 0x015A }, /* LATIN CAPITAL LETTER S WITH ACUTE */
        { "Scaron", 0x0160 }, /* LATIN CAPITAL LETTER S WITH CARON */
        { "Scaronsmall", 0xF6FD }, /* LATIN SMALL CAPITAL LETTER S WITH CARON */
        { "Scedilla" DUP1, 0x015E }, /* LATIN CAPITAL LETTER S WITH CEDILLA */
        { "Scedilla" DUP2, 0xF6C1 }, /* LATIN CAPITAL LETTER S WITH CEDILLA;Duplicate */
        { "Scircumflex", 0x015C }, /* LATIN CAPITAL LETTER S WITH CIRCUMFLEX */
        { "Scommaaccent", 0x0218 }, /* LATIN CAPITAL LETTER S WITH COMMA BELOW */
        { "Sigma", 0x03A3 }, /* GREEK CAPITAL LETTER SIGMA */
        { "Ssmall", 0xF773 }, /* LATIN SMALL CAPITAL LETTER S */
        { "T", 0x0054 }, /* LATIN CAPITAL LETTER T */
        { "Tau", 0x03A4 }, /* GREEK CAPITAL LETTER TAU */
        { "Tbar", 0x0166 }, /* LATIN CAPITAL LETTER T WITH STROKE */
        { "Tcaron", 0x0164 }, /* LATIN CAPITAL LETTER T WITH CARON */
        { "Tcommaaccent" DUP1, 0x0162 }, /* LATIN CAPITAL LETTER T WITH CEDILLA */
        { "Tcommaaccent" DUP2, 0x021A }, /* LATIN CAPITAL LETTER T WITH COMMA BELOW;Duplicate */
        { "Theta", 0x0398 }, /* GREEK CAPITAL LETTER THETA */
        { "Thorn", 0x00DE }, /* LATIN CAPITAL LETTER THORN */
        { "Thornsmall", 0xF7FE }, /* LATIN SMALL CAPITAL LETTER THORN */
        { "Tildesmall", 0xF6FE }, /* SMALL CAPITAL SMALL TILDE */
        { "Tsmall", 0xF774 }, /* LATIN SMALL CAPITAL LETTER T */
        { "U", 0x0055 }, /* LATIN CAPITAL LETTER U */
        { "Uacute", 0x00DA }, /* LATIN CAPITAL LETTER U WITH ACUTE */
        { "Uacutesmall", 0xF7FA }, /* LATIN SMALL CAPITAL LETTER U WITH ACUTE */
        { "Ubreve", 0x016C }, /* LATIN CAPITAL LETTER U WITH BREVE */
        { "Ucircumflex", 0x00DB }, /* LATIN CAPITAL LETTER U WITH CIRCUMFLEX */
        { "Ucircumflexsmall", 0xF7FB }, /* LATIN SMALL CAPITAL LETTER U WITH CIRCUMFLEX */
        { "Udieresis", 0x00DC }, /* LATIN CAPITAL LETTER U WITH DIAERESIS */
        { "Udieresissmall", 0xF7FC }, /* LATIN SMALL CAPITAL LETTER U WITH DIAERESIS */
        { "Ugrave", 0x00D9 }, /* LATIN CAPITAL LETTER U WITH GRAVE */
        { "Ugravesmall", 0xF7F9 }, /* LATIN SMALL CAPITAL LETTER U WITH GRAVE */
        { "Uhorn", 0x01AF }, /* LATIN CAPITAL LETTER U WITH HORN */
        { "Uhungarumlaut", 0x0170 }, /* LATIN CAPITAL LETTER U WITH DOUBLE ACUTE */
        { "Umacron", 0x016A }, /* LATIN CAPITAL LETTER U WITH MACRON */
        { "Uogonek", 0x0172 }, /* LATIN CAPITAL LETTER U WITH OGONEK */
        { "Upsilon", 0x03A5 }, /* GREEK CAPITAL LETTER UPSILON */
        { "Upsilon1", 0x03D2 }, /* GREEK UPSILON WITH HOOK SYMBOL */
        { "Upsilondieresis", 0x03AB }, /* GREEK CAPITAL LETTER UPSILON WITH DIALYTIKA */
        { "Upsilontonos", 0x038E }, /* GREEK CAPITAL LETTER UPSILON WITH TONOS */
        { "Uring", 0x016E }, /* LATIN CAPITAL LETTER U WITH RING ABOVE */
        { "Usmall", 0xF775 }, /* LATIN SMALL CAPITAL LETTER U */
        { "Utilde", 0x0168 }, /* LATIN CAPITAL LETTER U WITH TILDE */
        { "V", 0x0056 }, /* LATIN CAPITAL LETTER V */
        { "Vsmall", 0xF776 }, /* LATIN SMALL CAPITAL LETTER V */
        { "W", 0x0057 }, /* LATIN CAPITAL LETTER W */
        { "Wacute", 0x1E82 }, /* LATIN CAPITAL LETTER W WITH ACUTE */
        { "Wcircumflex", 0x0174 }, /* LATIN CAPITAL LETTER W WITH CIRCUMFLEX */
        { "Wdieresis", 0x1E84 }, /* LATIN CAPITAL LETTER W WITH DIAERESIS */
        { "Wgrave", 0x1E80 }, /* LATIN CAPITAL LETTER W WITH GRAVE */
        { "Wsmall", 0xF777 }, /* LATIN SMALL CAPITAL LETTER W */
        { "X", 0x0058 }, /* LATIN CAPITAL LETTER X */
        { "Xi", 0x039E }, /* GREEK CAPITAL LETTER XI */
        { "Xsmall", 0xF778 }, /* LATIN SMALL CAPITAL LETTER X */
        { "Y", 0x0059 }, /* LATIN CAPITAL LETTER Y */
        { "Yacute", 0x00DD }, /* LATIN CAPITAL LETTER Y WITH ACUTE */
        { "Yacutesmall", 0xF7FD }, /* LATIN SMALL CAPITAL LETTER Y WITH ACUTE */
        { "Ycircumflex", 0x0176 }, /* LATIN CAPITAL LETTER Y WITH CIRCUMFLEX */
        { "Ydieresis", 0x0178 }, /* LATIN CAPITAL LETTER Y WITH DIAERESIS */
        { "Ydieresissmall", 0xF7FF }, /* LATIN SMALL CAPITAL LETTER Y WITH DIAERESIS */
        { "Ygrave", 0x1EF2 }, /* LATIN CAPITAL LETTER Y WITH GRAVE */
        { "Ysmall", 0xF779 }, /* LATIN SMALL CAPITAL LETTER Y */
        { "Z", 0x005A }, /* LATIN CAPITAL LETTER Z */
        { "Zacute", 0x0179 }, /* LATIN CAPITAL LETTER Z WITH ACUTE */
        { "Zcaron", 0x017D }, /* LATIN CAPITAL LETTER Z WITH CARON */
        { "Zcaronsmall", 0xF6FF }, /* LATIN SMALL CAPITAL LETTER Z WITH CARON */
        { "Zdotaccent", 0x017B }, /* LATIN CAPITAL LETTER Z WITH DOT ABOVE */
        { "Zeta", 0x0396 }, /* GREEK CAPITAL LETTER ZETA */
        { "Zsmall", 0xF77A }, /* LATIN SMALL CAPITAL LETTER Z */
        { "a", 0x0061 }, /* LATIN SMALL LETTER A */
        { "aacute", 0x00E1 }, /* LATIN SMALL LETTER A WITH ACUTE */
        { "abreve", 0x0103 }, /* LATIN SMALL LETTER A WITH BREVE */
        { "acircumflex", 0x00E2 }, /* LATIN SMALL LETTER A WITH CIRCUMFLEX */
        { "acute", 0x00B4 }, /* ACUTE ACCENT */
        { "acutecomb", 0x0301 }, /* COMBINING ACUTE ACCENT */
        { "adieresis", 0x00E4 }, /* LATIN SMALL LETTER A WITH DIAERESIS */
        { "ae", 0x00E6 }, /* LATIN SMALL LETTER AE */
        { "aeacute", 0x01FD }, /* LATIN SMALL LETTER AE WITH ACUTE */
        { "afii00208", 0x2015 }, /* HORIZONTAL BAR */
        { "afii10017", 0x0410 }, /* CYRILLIC CAPITAL LETTER A */
        { "afii10018", 0x0411 }, /* CYRILLIC CAPITAL LETTER BE */
        { "afii10019", 0x0412 }, /* CYRILLIC CAPITAL LETTER VE */
        { "afii10020", 0x0413 }, /* CYRILLIC CAPITAL LETTER GHE */
        { "afii10021", 0x0414 }, /* CYRILLIC CAPITAL LETTER DE */
        { "afii10022", 0x0415 }, /* CYRILLIC CAPITAL LETTER IE */
        { "afii10023", 0x0401 }, /* CYRILLIC CAPITAL LETTER IO */
        { "afii10024", 0x0416 }, /* CYRILLIC CAPITAL LETTER ZHE */
        { "afii10025", 0x0417 }, /* CYRILLIC CAPITAL LETTER ZE */
        { "afii10026", 0x0418 }, /* CYRILLIC CAPITAL LETTER I */
        { "afii10027", 0x0419 }, /* CYRILLIC CAPITAL LETTER SHORT I */
        { "afii10028", 0x041A }, /* CYRILLIC CAPITAL LETTER KA */
        { "afii10029", 0x041B }, /* CYRILLIC CAPITAL LETTER EL */
        { "afii10030", 0x041C }, /* CYRILLIC CAPITAL LETTER EM */
        { "afii10031", 0x041D }, /* CYRILLIC CAPITAL LETTER EN */
        { "afii10032", 0x041E }, /* CYRILLIC CAPITAL LETTER O */
        { "afii10033", 0x041F }, /* CYRILLIC CAPITAL LETTER PE */
        { "afii10034", 0x0420 }, /* CYRILLIC CAPITAL LETTER ER */
        { "afii10035", 0x0421 }, /* CYRILLIC CAPITAL LETTER ES */
        { "afii10036", 0x0422 }, /* CYRILLIC CAPITAL LETTER TE */
        { "afii10037", 0x0423 }, /* CYRILLIC CAPITAL LETTER U */
        { "afii10038", 0x0424 }, /* CYRILLIC CAPITAL LETTER EF */
        { "afii10039", 0x0425 }, /* CYRILLIC CAPITAL LETTER HA */
        { "afii10040", 0x0426 }, /* CYRILLIC CAPITAL LETTER TSE */
        { "afii10041", 0x0427 }, /* CYRILLIC CAPITAL LETTER CHE */
        { "afii10042", 0x0428 }, /* CYRILLIC CAPITAL LETTER SHA */
        { "afii10043", 0x0429 }, /* CYRILLIC CAPITAL LETTER SHCHA */
        { "afii10044", 0x042A }, /* CYRILLIC CAPITAL LETTER HARD SIGN */
        { "afii10045", 0x042B }, /* CYRILLIC CAPITAL LETTER YERU */
        { "afii10046", 0x042C }, /* CYRILLIC CAPITAL LETTER SOFT SIGN */
        { "afii10047", 0x042D }, /* CYRILLIC CAPITAL LETTER E */
        { "afii10048", 0x042E }, /* CYRILLIC CAPITAL LETTER YU */
        { "afii10049", 0x042F }, /* CYRILLIC CAPITAL LETTER YA */
        { "afii10050", 0x0490 }, /* CYRILLIC CAPITAL LETTER GHE WITH UPTURN */
        { "afii10051", 0x0402 }, /* CYRILLIC CAPITAL LETTER DJE */
        { "afii10052", 0x0403 }, /* CYRILLIC CAPITAL LETTER GJE */
        { "afii10053", 0x0404 }, /* CYRILLIC CAPITAL LETTER UKRAINIAN IE */
        { "afii10054", 0x0405 }, /* CYRILLIC CAPITAL LETTER DZE */
        { "afii10055", 0x0406 }, /* CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I */
        { "afii10056", 0x0407 }, /* CYRILLIC CAPITAL LETTER YI */
        { "afii10057", 0x0408 }, /* CYRILLIC CAPITAL LETTER JE */
        { "afii10058", 0x0409 }, /* CYRILLIC CAPITAL LETTER LJE */
        { "afii10059", 0x040A }, /* CYRILLIC CAPITAL LETTER NJE */
        { "afii10060", 0x040B }, /* CYRILLIC CAPITAL LETTER TSHE */
        { "afii10061", 0x040C }, /* CYRILLIC CAPITAL LETTER KJE */
        { "afii10062", 0x040E }, /* CYRILLIC CAPITAL LETTER SHORT U */
        { "afii10063", 0xF6C4 }, /* CYRILLIC SMALL LETTER GHE VARIANT */
        { "afii10064", 0xF6C5 }, /* CYRILLIC SMALL LETTER BE VARIANT */
        { "afii10065", 0x0430 }, /* CYRILLIC SMALL LETTER A */
        { "afii10066", 0x0431 }, /* CYRILLIC SMALL LETTER BE */
        { "afii10067", 0x0432 }, /* CYRILLIC SMALL LETTER VE */
        { "afii10068", 0x0433 }, /* CYRILLIC SMALL LETTER GHE */
        { "afii10069", 0x0434 }, /* CYRILLIC SMALL LETTER DE */
        { "afii10070", 0x0435 }, /* CYRILLIC SMALL LETTER IE */
        { "afii10071", 0x0451 }, /* CYRILLIC SMALL LETTER IO */
        { "afii10072", 0x0436 }, /* CYRILLIC SMALL LETTER ZHE */
        { "afii10073", 0x0437 }, /* CYRILLIC SMALL LETTER ZE */
        { "afii10074", 0x0438 }, /* CYRILLIC SMALL LETTER I */
        { "afii10075", 0x0439 }, /* CYRILLIC SMALL LETTER SHORT I */
        { "afii10076", 0x043A }, /* CYRILLIC SMALL LETTER KA */
        { "afii10077", 0x043B }, /* CYRILLIC SMALL LETTER EL */
        { "afii10078", 0x043C }, /* CYRILLIC SMALL LETTER EM */
        { "afii10079", 0x043D }, /* CYRILLIC SMALL LETTER EN */
        { "afii10080", 0x043E }, /* CYRILLIC SMALL LETTER O */
        { "afii10081", 0x043F }, /* CYRILLIC SMALL LETTER PE */
        { "afii10082", 0x0440 }, /* CYRILLIC SMALL LETTER ER */
        { "afii10083", 0x0441 }, /* CYRILLIC SMALL LETTER ES */
        { "afii10084", 0x0442 }, /* CYRILLIC SMALL LETTER TE */
        { "afii10085", 0x0443 }, /* CYRILLIC SMALL LETTER U */
        { "afii10086", 0x0444 }, /* CYRILLIC SMALL LETTER EF */
        { "afii10087", 0x0445 }, /* CYRILLIC SMALL LETTER HA */
        { "afii10088", 0x0446 }, /* CYRILLIC SMALL LETTER TSE */
        { "afii10089", 0x0447 }, /* CYRILLIC SMALL LETTER CHE */
        { "afii10090", 0x0448 }, /* CYRILLIC SMALL LETTER SHA */
        { "afii10091", 0x0449 }, /* CYRILLIC SMALL LETTER SHCHA */
        { "afii10092", 0x044A }, /* CYRILLIC SMALL LETTER HARD SIGN */
        { "afii10093", 0x044B }, /* CYRILLIC SMALL LETTER YERU */
        { "afii10094", 0x044C }, /* CYRILLIC SMALL LETTER SOFT SIGN */
        { "afii10095", 0x044D }, /* CYRILLIC SMALL LETTER E */
        { "afii10096", 0x044E }, /* CYRILLIC SMALL LETTER YU */
        { "afii10097", 0x044F }, /* CYRILLIC SMALL LETTER YA */
        { "afii10098", 0x0491 }, /* CYRILLIC SMALL LETTER GHE WITH UPTURN */
        { "afii10099", 0x0452 }, /* CYRILLIC SMALL LETTER DJE */
        { "afii10100", 0x0453 }, /* CYRILLIC SMALL LETTER GJE */
        { "afii10101", 0x0454 }, /* CYRILLIC SMALL LETTER UKRAINIAN IE */
        { "afii10102", 0x0455 }, /* CYRILLIC SMALL LETTER DZE */
        { "afii10103", 0x0456 }, /* CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I */
        { "afii10104", 0x0457 }, /* CYRILLIC SMALL LETTER YI */
        { "afii10105", 0x0458 }, /* CYRILLIC SMALL LETTER JE */
        { "afii10106", 0x0459 }, /* CYRILLIC SMALL LETTER LJE */
        { "afii10107", 0x045A }, /* CYRILLIC SMALL LETTER NJE */
        { "afii10108", 0x045B }, /* CYRILLIC SMALL LETTER TSHE */
        { "afii10109", 0x045C }, /* CYRILLIC SMALL LETTER KJE */
        { "afii10110", 0x045E }, /* CYRILLIC SMALL LETTER SHORT U */
        { "afii10145", 0x040F }, /* CYRILLIC CAPITAL LETTER DZHE */
        { "afii10146", 0x0462 }, /* CYRILLIC CAPITAL LETTER YAT */
        { "afii10147", 0x0472 }, /* CYRILLIC CAPITAL LETTER FITA */
        { "afii10148", 0x0474 }, /* CYRILLIC CAPITAL LETTER IZHITSA */
        { "afii10192", 0xF6C6 }, /* CYRILLIC SMALL LETTER DE VARIANT */
        { "afii10193", 0x045F }, /* CYRILLIC SMALL LETTER DZHE */
        { "afii10194", 0x0463 }, /* CYRILLIC SMALL LETTER YAT */
        { "afii10195", 0x0473 }, /* CYRILLIC SMALL LETTER FITA */
        { "afii10196", 0x0475 }, /* CYRILLIC SMALL LETTER IZHITSA */
        { "afii10831", 0xF6C7 }, /* CYRILLIC SMALL LETTER PE VARIANT */
        { "afii10832", 0xF6C8 }, /* CYRILLIC SMALL LETTER TE VARIANT */
        { "afii10846", 0x04D9 }, /* CYRILLIC SMALL LETTER SCHWA */
        { "afii299", 0x200E }, /* LEFT-TO-RIGHT MARK */
        { "afii300", 0x200F }, /* RIGHT-TO-LEFT MARK */
        { "afii301", 0x200D }, /* ZERO WIDTH JOINER */
        { "afii57381", 0x066A }, /* ARABIC PERCENT SIGN */
        { "afii57388", 0x060C }, /* ARABIC COMMA */
        { "afii57392", 0x0660 }, /* ARABIC-INDIC DIGIT ZERO */
        { "afii57393", 0x0661 }, /* ARABIC-INDIC DIGIT ONE */
        { "afii57394", 0x0662 }, /* ARABIC-INDIC DIGIT TWO */
        { "afii57395", 0x0663 }, /* ARABIC-INDIC DIGIT THREE */
        { "afii57396", 0x0664 }, /* ARABIC-INDIC DIGIT FOUR */
        { "afii57397", 0x0665 }, /* ARABIC-INDIC DIGIT FIVE */
        { "afii57398", 0x0666 }, /* ARABIC-INDIC DIGIT SIX */
        { "afii57399", 0x0667 }, /* ARABIC-INDIC DIGIT SEVEN */
        { "afii57400", 0x0668 }, /* ARABIC-INDIC DIGIT EIGHT */
        { "afii57401", 0x0669 }, /* ARABIC-INDIC DIGIT NINE */
        { "afii57403", 0x061B }, /* ARABIC SEMICOLON */
        { "afii57407", 0x061F }, /* ARABIC QUESTION MARK */
        { "afii57409", 0x0621 }, /* ARABIC LETTER HAMZA */
        { "afii57410", 0x0622 }, /* ARABIC LETTER ALEF WITH MADDA ABOVE */
        { "afii57411", 0x0623 }, /* ARABIC LETTER ALEF WITH HAMZA ABOVE */
        { "afii57412", 0x0624 }, /* ARABIC LETTER WAW WITH HAMZA ABOVE */
        { "afii57413", 0x0625 }, /* ARABIC LETTER ALEF WITH HAMZA BELOW */
        { "afii57414", 0x0626 }, /* ARABIC LETTER YEH WITH HAMZA ABOVE */
        { "afii57415", 0x0627 }, /* ARABIC LETTER ALEF */
        { "afii57416", 0x0628 }, /* ARABIC LETTER BEH */
        { "afii57417", 0x0629 }, /* ARABIC LETTER TEH MARBUTA */
        { "afii57418", 0x062A }, /* ARABIC LETTER TEH */
        { "afii57419", 0x062B }, /* ARABIC LETTER THEH */
        { "afii57420", 0x062C }, /* ARABIC LETTER JEEM */
        { "afii57421", 0x062D }, /* ARABIC LETTER HAH */
        { "afii57422", 0x062E }, /* ARABIC LETTER KHAH */
        { "afii57423", 0x062F }, /* ARABIC LETTER DAL */
        { "afii57424", 0x0630 }, /* ARABIC LETTER THAL */
        { "afii57425", 0x0631 }, /* ARABIC LETTER REH */
        { "afii57426", 0x0632 }, /* ARABIC LETTER ZAIN */
        { "afii57427", 0x0633 }, /* ARABIC LETTER SEEN */
        { "afii57428", 0x0634 }, /* ARABIC LETTER SHEEN */
        { "afii57429", 0x0635 }, /* ARABIC LETTER SAD */
        { "afii57430", 0x0636 }, /* ARABIC LETTER DAD */
        { "afii57431", 0x0637 }, /* ARABIC LETTER TAH */
        { "afii57432", 0x0638 }, /* ARABIC LETTER ZAH */
        { "afii57433", 0x0639 }, /* ARABIC LETTER AIN */
        { "afii57434", 0x063A }, /* ARABIC LETTER GHAIN */
        { "afii57440", 0x0640 }, /* ARABIC TATWEEL */
        { "afii57441", 0x0641 }, /* ARABIC LETTER FEH */
        { "afii57442", 0x0642 }, /* ARABIC LETTER QAF */
        { "afii57443", 0x0643 }, /* ARABIC LETTER KAF */
        { "afii57444", 0x0644 }, /* ARABIC LETTER LAM */
        { "afii57445", 0x0645 }, /* ARABIC LETTER MEEM */
        { "afii57446", 0x0646 }, /* ARABIC LETTER NOON */
        { "afii57448", 0x0648 }, /* ARABIC LETTER WAW */
        { "afii57449", 0x0649 }, /* ARABIC LETTER ALEF MAKSURA */
        { "afii57450", 0x064A }, /* ARABIC LETTER YEH */
        { "afii57451", 0x064B }, /* ARABIC FATHATAN */
        { "afii57452", 0x064C }, /* ARABIC DAMMATAN */
        { "afii57453", 0x064D }, /* ARABIC KASRATAN */
        { "afii57454", 0x064E }, /* ARABIC FATHA */
        { "afii57455", 0x064F }, /* ARABIC DAMMA */
        { "afii57456", 0x0650 }, /* ARABIC KASRA */
        { "afii57457", 0x0651 }, /* ARABIC SHADDA */
        { "afii57458", 0x0652 }, /* ARABIC SUKUN */
        { "afii57470", 0x0647 }, /* ARABIC LETTER HEH */
        { "afii57505", 0x06A4 }, /* ARABIC LETTER VEH */
        { "afii57506", 0x067E }, /* ARABIC LETTER PEH */
        { "afii57507", 0x0686 }, /* ARABIC LETTER TCHEH */
        { "afii57508", 0x0698 }, /* ARABIC LETTER JEH */
        { "afii57509", 0x06AF }, /* ARABIC LETTER GAF */
        { "afii57511", 0x0679 }, /* ARABIC LETTER TTEH */
        { "afii57512", 0x0688 }, /* ARABIC LETTER DDAL */
        { "afii57513", 0x0691 }, /* ARABIC LETTER RREH */
        { "afii57514", 0x06BA }, /* ARABIC LETTER NOON GHUNNA */
        { "afii57519", 0x06D2 }, /* ARABIC LETTER YEH BARREE */
        { "afii57534", 0x06D5 }, /* ARABIC LETTER AE */
        { "afii57636", 0x20AA }, /* NEW SHEQEL SIGN */
        { "afii57645", 0x05BE }, /* HEBREW PUNCTUATION MAQAF */
        { "afii57658", 0x05C3 }, /* HEBREW PUNCTUATION SOF PASUQ */
        { "afii57664", 0x05D0 }, /* HEBREW LETTER ALEF */
        { "afii57665", 0x05D1 }, /* HEBREW LETTER BET */
        { "afii57666", 0x05D2 }, /* HEBREW LETTER GIMEL */
        { "afii57667", 0x05D3 }, /* HEBREW LETTER DALET */
        { "afii57668", 0x05D4 }, /* HEBREW LETTER HE */
        { "afii57669", 0x05D5 }, /* HEBREW LETTER VAV */
        { "afii57670", 0x05D6 }, /* HEBREW LETTER ZAYIN */
        { "afii57671", 0x05D7 }, /* HEBREW LETTER HET */
        { "afii57672", 0x05D8 }, /* HEBREW LETTER TET */
        { "afii57673", 0x05D9 }, /* HEBREW LETTER YOD */
        { "afii57674", 0x05DA }, /* HEBREW LETTER FINAL KAF */
        { "afii57675", 0x05DB }, /* HEBREW LETTER KAF */
        { "afii57676", 0x05DC }, /* HEBREW LETTER LAMED */
        { "afii57677", 0x05DD }, /* HEBREW LETTER FINAL MEM */
        { "afii57678", 0x05DE }, /* HEBREW LETTER MEM */
        { "afii57679", 0x05DF }, /* HEBREW LETTER FINAL NUN */
        { "afii57680", 0x05E0 }, /* HEBREW LETTER NUN */
        { "afii57681", 0x05E1 }, /* HEBREW LETTER SAMEKH */
        { "afii57682", 0x05E2 }, /* HEBREW LETTER AYIN */
        { "afii57683", 0x05E3 }, /* HEBREW LETTER FINAL PE */
        { "afii57684", 0x05E4 }, /* HEBREW LETTER PE */
        { "afii57685", 0x05E5 }, /* HEBREW LETTER FINAL TSADI */
        { "afii57686", 0x05E6 }, /* HEBREW LETTER TSADI */
        { "afii57687", 0x05E7 }, /* HEBREW LETTER QOF */
        { "afii57688", 0x05E8 }, /* HEBREW LETTER RESH */
        { "afii57689", 0x05E9 }, /* HEBREW LETTER SHIN */
        { "afii57690", 0x05EA }, /* HEBREW LETTER TAV */
        { "afii57694", 0xFB2A }, /* HEBREW LETTER SHIN WITH SHIN DOT */
        { "afii57695", 0xFB2B }, /* HEBREW LETTER SHIN WITH SIN DOT */
        { "afii57700", 0xFB4B }, /* HEBREW LETTER VAV WITH HOLAM */
        { "afii57705", 0xFB1F }, /* HEBREW LIGATURE YIDDISH YOD YOD PATAH */
        { "afii57716", 0x05F0 }, /* HEBREW LIGATURE YIDDISH DOUBLE VAV */
        { "afii57717", 0x05F1 }, /* HEBREW LIGATURE YIDDISH VAV YOD */
        { "afii57718", 0x05F2 }, /* HEBREW LIGATURE YIDDISH DOUBLE YOD */
        { "afii57723", 0xFB35 }, /* HEBREW LETTER VAV WITH DAGESH */
        { "afii57793", 0x05B4 }, /* HEBREW POINT HIRIQ */
        { "afii57794", 0x05B5 }, /* HEBREW POINT TSERE */
        { "afii57795", 0x05B6 }, /* HEBREW POINT SEGOL */
        { "afii57796", 0x05BB }, /* HEBREW POINT QUBUTS */
        { "afii57797", 0x05B8 }, /* HEBREW POINT QAMATS */
        { "afii57798", 0x05B7 }, /* HEBREW POINT PATAH */
        { "afii57799", 0x05B0 }, /* HEBREW POINT SHEVA */
        { "afii57800", 0x05B2 }, /* HEBREW POINT HATAF PATAH */
        { "afii57801", 0x05B1 }, /* HEBREW POINT HATAF SEGOL */
        { "afii57802", 0x05B3 }, /* HEBREW POINT HATAF QAMATS */
        { "afii57803", 0x05C2 }, /* HEBREW POINT SIN DOT */
        { "afii57804", 0x05C1 }, /* HEBREW POINT SHIN DOT */
        { "afii57806", 0x05B9 }, /* HEBREW POINT HOLAM */
        { "afii57807", 0x05BC }, /* HEBREW POINT DAGESH OR MAPIQ */
        { "afii57839", 0x05BD }, /* HEBREW POINT METEG */
        { "afii57841", 0x05BF }, /* HEBREW POINT RAFE */
        { "afii57842", 0x05C0 }, /* HEBREW PUNCTUATION PASEQ */
        { "afii57929", 0x02BC }, /* MODIFIER LETTER APOSTROPHE */
        { "afii61248", 0x2105 }, /* CARE OF */
        { "afii61289", 0x2113 }, /* SCRIPT SMALL L */
        { "afii61352", 0x2116 }, /* NUMERO SIGN */
        { "afii61573", 0x202C }, /* POP DIRECTIONAL FORMATTING */
        { "afii61574", 0x202D }, /* LEFT-TO-RIGHT OVERRIDE */
        { "afii61575", 0x202E }, /* RIGHT-TO-LEFT OVERRIDE */
        { "afii61664", 0x200C }, /* ZERO WIDTH NON-JOINER */
        { "afii63167", 0x066D }, /* ARABIC FIVE POINTED STAR */
        { "afii64937", 0x02BD }, /* MODIFIER LETTER REVERSED COMMA */
        { "agrave", 0x00E0 }, /* LATIN SMALL LETTER A WITH GRAVE */
        { "aleph", 0x2135 }, /* ALEF SYMBOL */
        { "alpha", 0x03B1 }, /* GREEK SMALL LETTER ALPHA */
        { "alphatonos", 0x03AC }, /* GREEK SMALL LETTER ALPHA WITH TONOS */
        { "amacron", 0x0101 }, /* LATIN SMALL LETTER A WITH MACRON */
        { "ampersand", 0x0026 }, /* AMPERSAND */
        { "ampersandsmall", 0xF726 }, /* SMALL CAPITAL AMPERSAND */
        { "angle", 0x2220 }, /* ANGLE */
        { "angleleft", 0x2329 }, /* LEFT-POINTING ANGLE BRACKET */
        { "angleright", 0x232A }, /* RIGHT-POINTING ANGLE BRACKET */
        { "anoteleia", 0x0387 }, /* GREEK ANO TELEIA */
        { "aogonek", 0x0105 }, /* LATIN SMALL LETTER A WITH OGONEK */
        { "approxequal", 0x2248 }, /* ALMOST EQUAL TO */
        { "aring", 0x00E5 }, /* LATIN SMALL LETTER A WITH RING ABOVE */
        { "aringacute", 0x01FB }, /* LATIN SMALL LETTER A WITH RING ABOVE AND ACUTE */
        { "arrowboth", 0x2194 }, /* LEFT RIGHT ARROW */
        { "arrowdblboth", 0x21D4 }, /* LEFT RIGHT DOUBLE ARROW */
        { "arrowdbldown", 0x21D3 }, /* DOWNWARDS DOUBLE ARROW */
        { "arrowdblleft", 0x21D0 }, /* LEFTWARDS DOUBLE ARROW */
        { "arrowdblright", 0x21D2 }, /* RIGHTWARDS DOUBLE ARROW */
        { "arrowdblup", 0x21D1 }, /* UPWARDS DOUBLE ARROW */
        { "arrowdown", 0x2193 }, /* DOWNWARDS ARROW */
        { "arrowhorizex", 0xF8E7 }, /* HORIZONTAL ARROW EXTENDER */
        { "arrowleft", 0x2190 }, /* LEFTWARDS ARROW */
        { "arrowright", 0x2192 }, /* RIGHTWARDS ARROW */
        { "arrowup", 0x2191 }, /* UPWARDS ARROW */
        { "arrowupdn", 0x2195 }, /* UP DOWN ARROW */
        { "arrowupdnbse", 0x21A8 }, /* UP DOWN ARROW WITH BASE */
        { "arrowvertex", 0xF8E6 }, /* VERTICAL ARROW EXTENDER */
        { "asciicircum", 0x005E }, /* CIRCUMFLEX ACCENT */
        { "asciitilde", 0x007E }, /* TILDE */
        { "asterisk", 0x002A }, /* ASTERISK */
        { "asteriskmath", 0x2217 }, /* ASTERISK OPERATOR */
        { "asuperior", 0xF6E9 }, /* SUPERSCRIPT LATIN SMALL LETTER A */
        { "at", 0x0040 }, /* COMMERCIAL AT */
        { "atilde", 0x00E3 }, /* LATIN SMALL LETTER A WITH TILDE */
        { "b", 0x0062 }, /* LATIN SMALL LETTER B */
        { "backslash", 0x005C }, /* REVERSE SOLIDUS */
        { "bar", 0x007C }, /* VERTICAL LINE */
        { "beta", 0x03B2 }, /* GREEK SMALL LETTER BETA */
        { "block", 0x2588 }, /* FULL BLOCK */
        { "braceex", 0xF8F4 }, /* CURLY BRACKET EXTENDER */
        { "braceleft", 0x007B }, /* LEFT CURLY BRACKET */
        { "braceleftbt", 0xF8F3 }, /* LEFT CURLY BRACKET BOTTOM */
        { "braceleftmid", 0xF8F2 }, /* LEFT CURLY BRACKET MID */
        { "bracelefttp", 0xF8F1 }, /* LEFT CURLY BRACKET TOP */
        { "braceright", 0x007D }, /* RIGHT CURLY BRACKET */
        { "bracerightbt", 0xF8FE }, /* RIGHT CURLY BRACKET BOTTOM */
        { "bracerightmid", 0xF8FD }, /* RIGHT CURLY BRACKET MID */
        { "bracerighttp", 0xF8FC }, /* RIGHT CURLY BRACKET TOP */
        { "bracketleft", 0x005B }, /* LEFT SQUARE BRACKET */
        { "bracketleftbt", 0xF8F0 }, /* LEFT SQUARE BRACKET BOTTOM */
        { "bracketleftex", 0xF8EF }, /* LEFT SQUARE BRACKET EXTENDER */
        { "bracketlefttp", 0xF8EE }, /* LEFT SQUARE BRACKET TOP */
        { "bracketright", 0x005D }, /* RIGHT SQUARE BRACKET */
        { "bracketrightbt", 0xF8FB }, /* RIGHT SQUARE BRACKET BOTTOM */
        { "bracketrightex", 0xF8FA }, /* RIGHT SQUARE BRACKET EXTENDER */
        { "bracketrighttp", 0xF8F9 }, /* RIGHT SQUARE BRACKET TOP */
        { "breve", 0x02D8 }, /* BREVE */
        { "brokenbar", 0x00A6 }, /* BROKEN BAR */
        { "bsuperior", 0xF6EA }, /* SUPERSCRIPT LATIN SMALL LETTER B */
        { "bullet", 0x2022 }, /* BULLET */
        { "c", 0x0063 }, /* LATIN SMALL LETTER C */
        { "cacute", 0x0107 }, /* LATIN SMALL LETTER C WITH ACUTE */
        { "caron", 0x02C7 }, /* CARON */
        { "carriagereturn", 0x21B5 }, /* DOWNWARDS ARROW WITH CORNER LEFTWARDS */
        { "ccaron", 0x010D }, /* LATIN SMALL LETTER C WITH CARON */
        { "ccedilla", 0x00E7 }, /* LATIN SMALL LETTER C WITH CEDILLA */
        { "ccircumflex", 0x0109 }, /* LATIN SMALL LETTER C WITH CIRCUMFLEX */
        { "cdotaccent", 0x010B }, /* LATIN SMALL LETTER C WITH DOT ABOVE */
        { "cedilla", 0x00B8 }, /* CEDILLA */
        { "cent", 0x00A2 }, /* CENT SIGN */
        { "centinferior", 0xF6DF }, /* SUBSCRIPT CENT SIGN */
        { "centoldstyle", 0xF7A2 }, /* OLDSTYLE CENT SIGN */
        { "centsuperior", 0xF6E0 }, /* SUPERSCRIPT CENT SIGN */
        { "chi", 0x03C7 }, /* GREEK SMALL LETTER CHI */
        { "circle", 0x25CB }, /* WHITE CIRCLE */
        { "circlemultiply", 0x2297 }, /* CIRCLED TIMES */
        { "circleplus", 0x2295 }, /* CIRCLED PLUS */
        { "circumflex", 0x02C6 }, /* MODIFIER LETTER CIRCUMFLEX ACCENT */
        { "club", 0x2663 }, /* BLACK CLUB SUIT */
        { "colon", 0x003A }, /* COLON */
        { "colonmonetary", 0x20A1 }, /* COLON SIGN */
        { "comma", 0x002C }, /* COMMA */
        { "commaaccent", 0xF6C3 }, /* COMMA BELOW */
        { "commainferior", 0xF6E1 }, /* SUBSCRIPT COMMA */
        { "commasuperior", 0xF6E2 }, /* SUPERSCRIPT COMMA */
        { "congruent", 0x2245 }, /* APPROXIMATELY EQUAL TO */
        { "copyright", 0x00A9 }, /* COPYRIGHT SIGN */
        { "copyrightsans", 0xF8E9 }, /* COPYRIGHT SIGN SANS SERIF */
        { "copyrightserif", 0xF6D9 }, /* COPYRIGHT SIGN SERIF */
        { "currency", 0x00A4 }, /* CURRENCY SIGN */
        { "cyrBreve", 0xF6D1 }, /* CAPITAL CYRILLIC BREVE */
        { "cyrFlex", 0xF6D2 }, /* CAPITAL CYRILLIC CIRCUMFLEX */
        { "cyrbreve", 0xF6D4 }, /* CYRILLIC BREVE */
        { "cyrflex", 0xF6D5 }, /* CYRILLIC CIRCUMFLEX */
        { "d", 0x0064 }, /* LATIN SMALL LETTER D */
        { "dagger", 0x2020 }, /* DAGGER */
        { "daggerdbl", 0x2021 }, /* DOUBLE DAGGER */
        { "dblGrave", 0xF6D3 }, /* CAPITAL DOUBLE GRAVE ACCENT */
        { "dblgrave", 0xF6D6 }, /* DOUBLE GRAVE ACCENT */
        { "dcaron", 0x010F }, /* LATIN SMALL LETTER D WITH CARON */
        { "dcroat", 0x0111 }, /* LATIN SMALL LETTER D WITH STROKE */
        { "degree", 0x00B0 }, /* DEGREE SIGN */
        { "delta", 0x03B4 }, /* GREEK SMALL LETTER DELTA */
        { "diamond", 0x2666 }, /* BLACK DIAMOND SUIT */
        { "dieresis", 0x00A8 }, /* DIAERESIS */
        { "dieresisacute", 0xF6D7 }, /* DIAERESIS ACUTE ACCENT */
        { "dieresisgrave", 0xF6D8 }, /* DIAERESIS GRAVE ACCENT */
        { "dieresistonos", 0x0385 }, /* GREEK DIALYTIKA TONOS */
        { "divide", 0x00F7 }, /* DIVISION SIGN */
        { "dkshade", 0x2593 }, /* DARK SHADE */
        { "dnblock", 0x2584 }, /* LOWER HALF BLOCK */
        { "dollar", 0x0024 }, /* DOLLAR SIGN */
        { "dollarinferior", 0xF6E3 }, /* SUBSCRIPT DOLLAR SIGN */
        { "dollaroldstyle", 0xF724 }, /* OLDSTYLE DOLLAR SIGN */
        { "dollarsuperior", 0xF6E4 }, /* SUPERSCRIPT DOLLAR SIGN */
        { "dong", 0x20AB }, /* DONG SIGN */
        { "dotaccent", 0x02D9 }, /* DOT ABOVE */
        { "dotbelowcomb", 0x0323 }, /* COMBINING DOT BELOW */
        { "dotlessi", 0x0131 }, /* LATIN SMALL LETTER DOTLESS I */
        { "dotlessj", 0xF6BE }, /* LATIN SMALL LETTER DOTLESS J */
        { "dotmath", 0x22C5 }, /* DOT OPERATOR */
        { "dsuperior", 0xF6EB }, /* SUPERSCRIPT LATIN SMALL LETTER D */
        { "e", 0x0065 }, /* LATIN SMALL LETTER E */
        { "eacute", 0x00E9 }, /* LATIN SMALL LETTER E WITH ACUTE */
        { "ebreve", 0x0115 }, /* LATIN SMALL LETTER E WITH BREVE */
        { "ecaron", 0x011B }, /* LATIN SMALL LETTER E WITH CARON */
        { "ecircumflex", 0x00EA }, /* LATIN SMALL LETTER E WITH CIRCUMFLEX */
        { "edieresis", 0x00EB }, /* LATIN SMALL LETTER E WITH DIAERESIS */
        { "edotaccent", 0x0117 }, /* LATIN SMALL LETTER E WITH DOT ABOVE */
        { "egrave", 0x00E8 }, /* LATIN SMALL LETTER E WITH GRAVE */
        { "eight", 0x0038 }, /* DIGIT EIGHT */
        { "eightinferior", 0x2088 }, /* SUBSCRIPT EIGHT */
        { "eightoldstyle", 0xF738 }, /* OLDSTYLE DIGIT EIGHT */
        { "eightsuperior", 0x2078 }, /* SUPERSCRIPT EIGHT */
        { "element", 0x2208 }, /* ELEMENT OF */
        { "ellipsis", 0x2026 }, /* HORIZONTAL ELLIPSIS */
        { "emacron", 0x0113 }, /* LATIN SMALL LETTER E WITH MACRON */
        { "emdash", 0x2014 }, /* EM DASH */
        { "emptyset", 0x2205 }, /* EMPTY SET */
        { "endash", 0x2013 }, /* EN DASH */
        { "eng", 0x014B }, /* LATIN SMALL LETTER ENG */
        { "eogonek", 0x0119 }, /* LATIN SMALL LETTER E WITH OGONEK */
        { "epsilon", 0x03B5 }, /* GREEK SMALL LETTER EPSILON */
        { "epsilontonos", 0x03AD }, /* GREEK SMALL LETTER EPSILON WITH TONOS */
        { "equal", 0x003D }, /* EQUALS SIGN */
        { "equivalence", 0x2261 }, /* IDENTICAL TO */
        { "estimated", 0x212E }, /* ESTIMATED SYMBOL */
        { "esuperior", 0xF6EC }, /* SUPERSCRIPT LATIN SMALL LETTER E */
        { "eta", 0x03B7 }, /* GREEK SMALL LETTER ETA */
        { "etatonos", 0x03AE }, /* GREEK SMALL LETTER ETA WITH TONOS */
        { "eth", 0x00F0 }, /* LATIN SMALL LETTER ETH */
        { "exclam", 0x0021 }, /* EXCLAMATION MARK */
        { "exclamdbl", 0x203C }, /* DOUBLE EXCLAMATION MARK */
        { "exclamdown", 0x00A1 }, /* INVERTED EXCLAMATION MARK */
        { "exclamdownsmall", 0xF7A1 }, /* SMALL CAPITAL INVERTED EXCLAMATION MARK */
        { "exclamsmall", 0xF721 }, /* SMALL CAPITAL EXCLAMATION MARK */
        { "existential", 0x2203 }, /* THERE EXISTS */
        { "f", 0x0066 }, /* LATIN SMALL LETTER F */
        { "female", 0x2640 }, /* FEMALE SIGN */
        { "ff", 0xFB00 }, /* LATIN SMALL LIGATURE FF */
        { "ffi", 0xFB03 }, /* LATIN SMALL LIGATURE FFI */
        { "ffl", 0xFB04 }, /* LATIN SMALL LIGATURE FFL */
        { "fi", 0xFB01 }, /* LATIN SMALL LIGATURE FI */
        { "figuredash", 0x2012 }, /* FIGURE DASH */
        { "filledbox", 0x25A0 }, /* BLACK SQUARE */
        { "filledrect", 0x25AC }, /* BLACK RECTANGLE */
        { "five", 0x0035 }, /* DIGIT FIVE */
        { "fiveeighths", 0x215D }, /* VULGAR FRACTION FIVE EIGHTHS */
        { "fiveinferior", 0x2085 }, /* SUBSCRIPT FIVE */
        { "fiveoldstyle", 0xF735 }, /* OLDSTYLE DIGIT FIVE */
        { "fivesuperior", 0x2075 }, /* SUPERSCRIPT FIVE */
        { "fl", 0xFB02 }, /* LATIN SMALL LIGATURE FL */
        { "florin", 0x0192 }, /* LATIN SMALL LETTER F WITH HOOK */
        { "four", 0x0034 }, /* DIGIT FOUR */
        { "fourinferior", 0x2084 }, /* SUBSCRIPT FOUR */
        { "fouroldstyle", 0xF734 }, /* OLDSTYLE DIGIT FOUR */
        { "foursuperior", 0x2074 }, /* SUPERSCRIPT FOUR */
        { "fraction" DUP1, 0x2044 }, /* FRACTION SLASH */
        { "fraction" DUP2, 0x2215 }, /* DIVISION SLASH;Duplicate */
        { "franc", 0x20A3 }, /* FRENCH FRANC SIGN */
        { "g", 0x0067 }, /* LATIN SMALL LETTER G */
        { "gamma", 0x03B3 }, /* GREEK SMALL LETTER GAMMA */
        { "gbreve", 0x011F }, /* LATIN SMALL LETTER G WITH BREVE */
        { "gcaron", 0x01E7 }, /* LATIN SMALL LETTER G WITH CARON */
        { "gcircumflex", 0x011D }, /* LATIN SMALL LETTER G WITH CIRCUMFLEX */
        { "gcommaaccent", 0x0123 }, /* LATIN SMALL LETTER G WITH CEDILLA */
        { "gdotaccent", 0x0121 }, /* LATIN SMALL LETTER G WITH DOT ABOVE */
        { "germandbls", 0x00DF }, /* LATIN SMALL LETTER SHARP S */
        { "gradient", 0x2207 }, /* NABLA */
        { "grave", 0x0060 }, /* GRAVE ACCENT */
        { "gravecomb", 0x0300 }, /* COMBINING GRAVE ACCENT */
        { "greater", 0x003E }, /* GREATER-THAN SIGN */
        { "greaterequal", 0x2265 }, /* GREATER-THAN OR EQUAL TO */
        { "guillemotleft", 0x00AB }, /* LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */
        { "guillemotright", 0x00BB }, /* RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */
        { "guilsinglleft", 0x2039 }, /* SINGLE LEFT-POINTING ANGLE QUOTATION MARK */
        { "guilsinglright", 0x203A }, /* SINGLE RIGHT-POINTING ANGLE QUOTATION MARK */
        { "h", 0x0068 }, /* LATIN SMALL LETTER H */
        { "hbar", 0x0127 }, /* LATIN SMALL LETTER H WITH STROKE */
        { "hcircumflex", 0x0125 }, /* LATIN SMALL LETTER H WITH CIRCUMFLEX */
        { "heart", 0x2665 }, /* BLACK HEART SUIT */
        { "hookabovecomb", 0x0309 }, /* COMBINING HOOK ABOVE */
        { "house", 0x2302 }, /* HOUSE */
        { "hungarumlaut", 0x02DD }, /* DOUBLE ACUTE ACCENT */
        { "hyphen" DUP1, 0x002D }, /* HYPHEN-MINUS */
        { "hyphen" DUP2, 0x00AD }, /* SOFT HYPHEN;Duplicate */
        { "hypheninferior", 0xF6E5 }, /* SUBSCRIPT HYPHEN-MINUS */
        { "hyphensuperior", 0xF6E6 }, /* SUPERSCRIPT HYPHEN-MINUS */
        { "i", 0x0069 }, /* LATIN SMALL LETTER I */
        { "iacute", 0x00ED }, /* LATIN SMALL LETTER I WITH ACUTE */
        { "ibreve", 0x012D }, /* LATIN SMALL LETTER I WITH BREVE */
        { "icircumflex", 0x00EE }, /* LATIN SMALL LETTER I WITH CIRCUMFLEX */
        { "idieresis", 0x00EF }, /* LATIN SMALL LETTER I WITH DIAERESIS */
        { "igrave", 0x00EC }, /* LATIN SMALL LETTER I WITH GRAVE */
        { "ij", 0x0133 }, /* LATIN SMALL LIGATURE IJ */
        { "imacron", 0x012B }, /* LATIN SMALL LETTER I WITH MACRON */
        { "infinity", 0x221E }, /* INFINITY */
        { "integral", 0x222B }, /* INTEGRAL */
        { "integralbt", 0x2321 }, /* BOTTOM HALF INTEGRAL */
        { "integralex", 0xF8F5 }, /* INTEGRAL EXTENDER */
        { "integraltp", 0x2320 }, /* TOP HALF INTEGRAL */
        { "intersection", 0x2229 }, /* INTERSECTION */
        { "invbullet", 0x25D8 }, /* INVERSE BULLET */
        { "invcircle", 0x25D9 }, /* INVERSE WHITE CIRCLE */
        { "invsmileface", 0x263B }, /* BLACK SMILING FACE */
        { "iogonek", 0x012F }, /* LATIN SMALL LETTER I WITH OGONEK */
        { "iota", 0x03B9 }, /* GREEK SMALL LETTER IOTA */
        { "iotadieresis", 0x03CA }, /* GREEK SMALL LETTER IOTA WITH DIALYTIKA */
        { "iotadieresistonos", 0x0390 }, /* GREEK SMALL LETTER IOTA WITH DIALYTIKA AND TONOS */
        { "iotatonos", 0x03AF }, /* GREEK SMALL LETTER IOTA WITH TONOS */
        { "isuperior", 0xF6ED }, /* SUPERSCRIPT LATIN SMALL LETTER I */
        { "itilde", 0x0129 }, /* LATIN SMALL LETTER I WITH TILDE */
        { "j", 0x006A }, /* LATIN SMALL LETTER J */
        { "jcircumflex", 0x0135 }, /* LATIN SMALL LETTER J WITH CIRCUMFLEX */
        { "k", 0x006B }, /* LATIN SMALL LETTER K */
        { "kappa", 0x03BA }, /* GREEK SMALL LETTER KAPPA */
        { "kcommaaccent", 0x0137 }, /* LATIN SMALL LETTER K WITH CEDILLA */
        { "kgreenlandic", 0x0138 }, /* LATIN SMALL LETTER KRA */
        { "l", 0x006C }, /* LATIN SMALL LETTER L */
        { "lacute", 0x013A }, /* LATIN SMALL LETTER L WITH ACUTE */
        { "lambda", 0x03BB }, /* GREEK SMALL LETTER LAMDA */
        { "lcaron", 0x013E }, /* LATIN SMALL LETTER L WITH CARON */
        { "lcommaaccent", 0x013C }, /* LATIN SMALL LETTER L WITH CEDILLA */
        { "ldot", 0x0140 }, /* LATIN SMALL LETTER L WITH MIDDLE DOT */
        { "less", 0x003C }, /* LESS-THAN SIGN */
        { "lessequal", 0x2264 }, /* LESS-THAN OR EQUAL TO */
        { "lfblock", 0x258C }, /* LEFT HALF BLOCK */
        { "lira", 0x20A4 }, /* LIRA SIGN */
        { "ll", 0xF6C0 }, /* LATIN SMALL LETTER LL */
        { "logicaland", 0x2227 }, /* LOGICAL AND */
        { "logicalnot", 0x00AC }, /* NOT SIGN */
        { "logicalor", 0x2228 }, /* LOGICAL OR */
        { "longs", 0x017F }, /* LATIN SMALL LETTER LONG S */
        { "lozenge", 0x25CA }, /* LOZENGE */
        { "lslash", 0x0142 }, /* LATIN SMALL LETTER L WITH STROKE */
        { "lsuperior", 0xF6EE }, /* SUPERSCRIPT LATIN SMALL LETTER L */
        { "ltshade", 0x2591 }, /* LIGHT SHADE */
        { "m", 0x006D }, /* LATIN SMALL LETTER M */
        { "macron" DUP1, 0x00AF }, /* MACRON */
        { "macron" DUP2, 0x02C9 }, /* MODIFIER LETTER MACRON;Duplicate */
        { "male", 0x2642 }, /* MALE SIGN */
        { "minus", 0x2212 }, /* MINUS SIGN */
        { "minute", 0x2032 }, /* PRIME */
        { "msuperior", 0xF6EF }, /* SUPERSCRIPT LATIN SMALL LETTER M */
        { "mu" DUP1, 0x00B5 }, /* MICRO SIGN */
        { "mu" DUP2, 0x03BC }, /* GREEK SMALL LETTER MU;Duplicate */
        { "multiply", 0x00D7 }, /* MULTIPLICATION SIGN */
        { "musicalnote", 0x266A }, /* EIGHTH NOTE */
        { "musicalnotedbl", 0x266B }, /* BEAMED EIGHTH NOTES */
        { "n", 0x006E }, /* LATIN SMALL LETTER N */
        { "nacute", 0x0144 }, /* LATIN SMALL LETTER N WITH ACUTE */
        { "napostrophe", 0x0149 }, /* LATIN SMALL LETTER N PRECEDED BY APOSTROPHE */
        { "ncaron", 0x0148 }, /* LATIN SMALL LETTER N WITH CARON */
        { "ncommaaccent", 0x0146 }, /* LATIN SMALL LETTER N WITH CEDILLA */
        { "nine", 0x0039 }, /* DIGIT NINE */
        { "nineinferior", 0x2089 }, /* SUBSCRIPT NINE */
        { "nineoldstyle", 0xF739 }, /* OLDSTYLE DIGIT NINE */
        { "ninesuperior", 0x2079 }, /* SUPERSCRIPT NINE */
        { "notelement", 0x2209 }, /* NOT AN ELEMENT OF */
        { "notequal", 0x2260 }, /* NOT EQUAL TO */
        { "notsubset", 0x2284 }, /* NOT A SUBSET OF */
        { "nsuperior", 0x207F }, /* SUPERSCRIPT LATIN SMALL LETTER N */
        { "ntilde", 0x00F1 }, /* LATIN SMALL LETTER N WITH TILDE */
        { "nu", 0x03BD }, /* GREEK SMALL LETTER NU */
        { "numbersign", 0x0023 }, /* NUMBER SIGN */
        { "o", 0x006F }, /* LATIN SMALL LETTER O */
        { "oacute", 0x00F3 }, /* LATIN SMALL LETTER O WITH ACUTE */
        { "obreve", 0x014F }, /* LATIN SMALL LETTER O WITH BREVE */
        { "ocircumflex", 0x00F4 }, /* LATIN SMALL LETTER O WITH CIRCUMFLEX */
        { "odieresis", 0x00F6 }, /* LATIN SMALL LETTER O WITH DIAERESIS */
        { "oe", 0x0153 }, /* LATIN SMALL LIGATURE OE */
        { "ogonek", 0x02DB }, /* OGONEK */
        { "ograve", 0x00F2 }, /* LATIN SMALL LETTER O WITH GRAVE */
        { "ohorn", 0x01A1 }, /* LATIN SMALL LETTER O WITH HORN */
        { "ohungarumlaut", 0x0151 }, /* LATIN SMALL LETTER O WITH DOUBLE ACUTE */
        { "omacron", 0x014D }, /* LATIN SMALL LETTER O WITH MACRON */
        { "omega", 0x03C9 }, /* GREEK SMALL LETTER OMEGA */
        { "omega1", 0x03D6 }, /* GREEK PI SYMBOL */
        { "omegatonos", 0x03CE }, /* GREEK SMALL LETTER OMEGA WITH TONOS */
        { "omicron", 0x03BF }, /* GREEK SMALL LETTER OMICRON */
        { "omicrontonos", 0x03CC }, /* GREEK SMALL LETTER OMICRON WITH TONOS */
        { "one", 0x0031 }, /* DIGIT ONE */
        { "onedotenleader", 0x2024 }, /* ONE DOT LEADER */
        { "oneeighth", 0x215B }, /* VULGAR FRACTION ONE EIGHTH */
        { "onefitted", 0xF6DC }, /* PROPORTIONAL DIGIT ONE */
        { "onehalf", 0x00BD }, /* VULGAR FRACTION ONE HALF */
        { "oneinferior", 0x2081 }, /* SUBSCRIPT ONE */
        { "oneoldstyle", 0xF731 }, /* OLDSTYLE DIGIT ONE */
        { "onequarter", 0x00BC }, /* VULGAR FRACTION ONE QUARTER */
        { "onesuperior", 0x00B9 }, /* SUPERSCRIPT ONE */
        { "onethird", 0x2153 }, /* VULGAR FRACTION ONE THIRD */
        { "openbullet", 0x25E6 }, /* WHITE BULLET */
        { "ordfeminine", 0x00AA }, /* FEMININE ORDINAL INDICATOR */
        { "ordmasculine", 0x00BA }, /* MASCULINE ORDINAL INDICATOR */
        { "orthogonal", 0x221F }, /* RIGHT ANGLE */
        { "oslash", 0x00F8 }, /* LATIN SMALL LETTER O WITH STROKE */
        { "oslashacute", 0x01FF }, /* LATIN SMALL LETTER O WITH STROKE AND ACUTE */
        { "osuperior", 0xF6F0 }, /* SUPERSCRIPT LATIN SMALL LETTER O */
        { "otilde", 0x00F5 }, /* LATIN SMALL LETTER O WITH TILDE */
        { "p", 0x0070 }, /* LATIN SMALL LETTER P */
        { "paragraph", 0x00B6 }, /* PILCROW SIGN */
        { "parenleft", 0x0028 }, /* LEFT PARENTHESIS */
        { "parenleftbt", 0xF8ED }, /* LEFT PAREN BOTTOM */
        { "parenleftex", 0xF8EC }, /* LEFT PAREN EXTENDER */
        { "parenleftinferior", 0x208D }, /* SUBSCRIPT LEFT PARENTHESIS */
        { "parenleftsuperior", 0x207D }, /* SUPERSCRIPT LEFT PARENTHESIS */
        { "parenlefttp", 0xF8EB }, /* LEFT PAREN TOP */
        { "parenright", 0x0029 }, /* RIGHT PARENTHESIS */
        { "parenrightbt", 0xF8F8 }, /* RIGHT PAREN BOTTOM */
        { "parenrightex", 0xF8F7 }, /* RIGHT PAREN EXTENDER */
        { "parenrightinferior", 0x208E }, /* SUBSCRIPT RIGHT PARENTHESIS */
        { "parenrightsuperior", 0x207E }, /* SUPERSCRIPT RIGHT PARENTHESIS */
        { "parenrighttp", 0xF8F6 }, /* RIGHT PAREN TOP */
        { "partialdiff", 0x2202 }, /* PARTIAL DIFFERENTIAL */
        { "percent", 0x0025 }, /* PERCENT SIGN */
        { "period", 0x002E }, /* FULL STOP */
        { "periodcentered" DUP1, 0x00B7 }, /* MIDDLE DOT */
        { "periodcentered" DUP2, 0x2219 }, /* BULLET OPERATOR;Duplicate */
        { "periodinferior", 0xF6E7 }, /* SUBSCRIPT FULL STOP */
        { "periodsuperior", 0xF6E8 }, /* SUPERSCRIPT FULL STOP */
        { "perpendicular", 0x22A5 }, /* UP TACK */
        { "perthousand", 0x2030 }, /* PER MILLE SIGN */
        { "peseta", 0x20A7 }, /* PESETA SIGN */
        { "phi", 0x03C6 }, /* GREEK SMALL LETTER PHI */
        { "phi1", 0x03D5 }, /* GREEK PHI SYMBOL */
        { "pi", 0x03C0 }, /* GREEK SMALL LETTER PI */
        { "plus", 0x002B }, /* PLUS SIGN */
        { "plusminus", 0x00B1 }, /* PLUS-MINUS SIGN */
        { "prescription", 0x211E }, /* PRESCRIPTION TAKE */
        { "product", 0x220F }, /* N-ARY PRODUCT */
        { "propersubset", 0x2282 }, /* SUBSET OF */
        { "propersuperset", 0x2283 }, /* SUPERSET OF */
        { "proportional", 0x221D }, /* PROPORTIONAL TO */
        { "psi", 0x03C8 }, /* GREEK SMALL LETTER PSI */
        { "q", 0x0071 }, /* LATIN SMALL LETTER Q */
        { "question", 0x003F }, /* QUESTION MARK */
        { "questiondown", 0x00BF }, /* INVERTED QUESTION MARK */
        { "questiondownsmall", 0xF7BF }, /* SMALL CAPITAL INVERTED QUESTION MARK */
        { "questionsmall", 0xF73F }, /* SMALL CAPITAL QUESTION MARK */
        { "quotedbl", 0x0022 }, /* QUOTATION MARK */
        { "quotedblbase", 0x201E }, /* DOUBLE LOW-9 QUOTATION MARK */
        { "quotedblleft", 0x201C }, /* LEFT DOUBLE QUOTATION MARK */
        { "quotedblright", 0x201D }, /* RIGHT DOUBLE QUOTATION MARK */
        { "quoteleft", 0x2018 }, /* LEFT SINGLE QUOTATION MARK */
        { "quotereversed", 0x201B }, /* SINGLE HIGH-REVERSED-9 QUOTATION MARK */
        { "quoteright", 0x2019 }, /* RIGHT SINGLE QUOTATION MARK */
        { "quotesinglbase", 0x201A }, /* SINGLE LOW-9 QUOTATION MARK */
        { "quotesingle", 0x0027 }, /* APOSTROPHE */
        { "r", 0x0072 }, /* LATIN SMALL LETTER R */
        { "racute", 0x0155 }, /* LATIN SMALL LETTER R WITH ACUTE */
        { "radical", 0x221A }, /* SQUARE ROOT */
        { "radicalex", 0xF8E5 }, /* RADICAL EXTENDER */
        { "rcaron", 0x0159 }, /* LATIN SMALL LETTER R WITH CARON */
        { "rcommaaccent", 0x0157 }, /* LATIN SMALL LETTER R WITH CEDILLA */
        { "reflexsubset", 0x2286 }, /* SUBSET OF OR EQUAL TO */
        { "reflexsuperset", 0x2287 }, /* SUPERSET OF OR EQUAL TO */
        { "registered", 0x00AE }, /* REGISTERED SIGN */
        { "registersans", 0xF8E8 }, /* REGISTERED SIGN SANS SERIF */
        { "registerserif", 0xF6DA }, /* REGISTERED SIGN SERIF */
        { "revlogicalnot", 0x2310 }, /* REVERSED NOT SIGN */
        { "rho", 0x03C1 }, /* GREEK SMALL LETTER RHO */
        { "ring", 0x02DA }, /* RING ABOVE */
        { "rsuperior", 0xF6F1 }, /* SUPERSCRIPT LATIN SMALL LETTER R */
        { "rtblock", 0x2590 }, /* RIGHT HALF BLOCK */
        { "rupiah", 0xF6DD }, /* RUPIAH SIGN */
        { "s", 0x0073 }, /* LATIN SMALL LETTER S */
        { "sacute", 0x015B }, /* LATIN SMALL LETTER S WITH ACUTE */
        { "scaron", 0x0161 }, /* LATIN SMALL LETTER S WITH CARON */
        { "scedilla" DUP1, 0x015F }, /* LATIN SMALL LETTER S WITH CEDILLA */
        { "scedilla" DUP2, 0xF6C2 }, /* LATIN SMALL LETTER S WITH CEDILLA;Duplicate */
        { "scircumflex", 0x015D }, /* LATIN SMALL LETTER S WITH CIRCUMFLEX */
        { "scommaaccent", 0x0219 }, /* LATIN SMALL LETTER S WITH COMMA BELOW */
        { "second", 0x2033 }, /* DOUBLE PRIME */
        { "section", 0x00A7 }, /* SECTION SIGN */
        { "semicolon", 0x003B }, /* SEMICOLON */
        { "seven", 0x0037 }, /* DIGIT SEVEN */
        { "seveneighths", 0x215E }, /* VULGAR FRACTION SEVEN EIGHTHS */
        { "seveninferior", 0x2087 }, /* SUBSCRIPT SEVEN */
        { "sevenoldstyle", 0xF737 }, /* OLDSTYLE DIGIT SEVEN */
        { "sevensuperior", 0x2077 }, /* SUPERSCRIPT SEVEN */
        { "shade", 0x2592 }, /* MEDIUM SHADE */
        { "sigma", 0x03C3 }, /* GREEK SMALL LETTER SIGMA */
        { "sigma1", 0x03C2 }, /* GREEK SMALL LETTER FINAL SIGMA */
        { "similar", 0x223C }, /* TILDE OPERATOR */
        { "six", 0x0036 }, /* DIGIT SIX */
        { "sixinferior", 0x2086 }, /* SUBSCRIPT SIX */
        { "sixoldstyle", 0xF736 }, /* OLDSTYLE DIGIT SIX */
        { "sixsuperior", 0x2076 }, /* SUPERSCRIPT SIX */
        { "slash", 0x002F }, /* SOLIDUS */
        { "smileface", 0x263A }, /* WHITE SMILING FACE */
        { "space" DUP1, 0x0020 }, /* SPACE */
        { "space" DUP2, 0x00A0 }, /* NO-BREAK SPACE;Duplicate */
        { "spade", 0x2660 }, /* BLACK SPADE SUIT */
        { "ssuperior", 0xF6F2 }, /* SUPERSCRIPT LATIN SMALL LETTER S */
        { "sterling", 0x00A3 }, /* POUND SIGN */
        { "suchthat", 0x220B }, /* CONTAINS AS MEMBER */
        { "summation", 0x2211 }, /* N-ARY SUMMATION */
        { "sun", 0x263C }, /* WHITE SUN WITH RAYS */
        { "t", 0x0074 }, /* LATIN SMALL LETTER T */
        { "tau", 0x03C4 }, /* GREEK SMALL LETTER TAU */
        { "tbar", 0x0167 }, /* LATIN SMALL LETTER T WITH STROKE */
        { "tcaron", 0x0165 }, /* LATIN SMALL LETTER T WITH CARON */
        { "tcommaaccent" DUP1, 0x0163 }, /* LATIN SMALL LETTER T WITH CEDILLA */
        { "tcommaaccent" DUP2, 0x021B }, /* LATIN SMALL LETTER T WITH COMMA BELOW;Duplicate */
        { "therefore", 0x2234 }, /* THEREFORE */
        { "theta", 0x03B8 }, /* GREEK SMALL LETTER THETA */
        { "theta1", 0x03D1 }, /* GREEK THETA SYMBOL */
        { "thorn", 0x00FE }, /* LATIN SMALL LETTER THORN */
        { "three", 0x0033 }, /* DIGIT THREE */
        { "threeeighths", 0x215C }, /* VULGAR FRACTION THREE EIGHTHS */
        { "threeinferior", 0x2083 }, /* SUBSCRIPT THREE */
        { "threeoldstyle", 0xF733 }, /* OLDSTYLE DIGIT THREE */
        { "threequarters", 0x00BE }, /* VULGAR FRACTION THREE QUARTERS */
        { "threequartersemdash", 0xF6DE }, /* THREE QUARTERS EM DASH */
        { "threesuperior", 0x00B3 }, /* SUPERSCRIPT THREE */
        { "tilde", 0x02DC }, /* SMALL TILDE */
        { "tildecomb", 0x0303 }, /* COMBINING TILDE */
        { "tonos", 0x0384 }, /* GREEK TONOS */
        { "trademark", 0x2122 }, /* TRADE MARK SIGN */
        { "trademarksans", 0xF8EA }, /* TRADE MARK SIGN SANS SERIF */
        { "trademarkserif", 0xF6DB }, /* TRADE MARK SIGN SERIF */
        { "triagdn", 0x25BC }, /* BLACK DOWN-POINTING TRIANGLE */
        { "triaglf", 0x25C4 }, /* BLACK LEFT-POINTING POINTER */
        { "triagrt", 0x25BA }, /* BLACK RIGHT-POINTING POINTER */
        { "triagup", 0x25B2 }, /* BLACK UP-POINTING TRIANGLE */
        { "tsuperior", 0xF6F3 }, /* SUPERSCRIPT LATIN SMALL LETTER T */
        { "two", 0x0032 }, /* DIGIT TWO */
        { "twodotenleader", 0x2025 }, /* TWO DOT LEADER */
        { "twoinferior", 0x2082 }, /* SUBSCRIPT TWO */
        { "twooldstyle", 0xF732 }, /* OLDSTYLE DIGIT TWO */
        { "twosuperior", 0x00B2 }, /* SUPERSCRIPT TWO */
        { "twothirds", 0x2154 }, /* VULGAR FRACTION TWO THIRDS */
        { "u", 0x0075 }, /* LATIN SMALL LETTER U */
        { "uacute", 0x00FA }, /* LATIN SMALL LETTER U WITH ACUTE */
        { "ubreve", 0x016D }, /* LATIN SMALL LETTER U WITH BREVE */
        { "ucircumflex", 0x00FB }, /* LATIN SMALL LETTER U WITH CIRCUMFLEX */
        { "udieresis", 0x00FC }, /* LATIN SMALL LETTER U WITH DIAERESIS */
        { "ugrave", 0x00F9 }, /* LATIN SMALL LETTER U WITH GRAVE */
        { "uhorn", 0x01B0 }, /* LATIN SMALL LETTER U WITH HORN */
        { "uhungarumlaut", 0x0171 }, /* LATIN SMALL LETTER U WITH DOUBLE ACUTE */
        { "umacron", 0x016B }, /* LATIN SMALL LETTER U WITH MACRON */
        { "underscore", 0x005F }, /* LOW LINE */
        { "underscoredbl", 0x2017 }, /* DOUBLE LOW LINE */
        { "union", 0x222A }, /* UNION */
        { "universal", 0x2200 }, /* FOR ALL */
        { "uogonek", 0x0173 }, /* LATIN SMALL LETTER U WITH OGONEK */
        { "upblock", 0x2580 }, /* UPPER HALF BLOCK */
        { "upsilon", 0x03C5 }, /* GREEK SMALL LETTER UPSILON */
        { "upsilondieresis", 0x03CB }, /* GREEK SMALL LETTER UPSILON WITH DIALYTIKA */
        { "upsilondieresistonos", 0x03B0 }, /* GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND TONOS */
        { "upsilontonos", 0x03CD }, /* GREEK SMALL LETTER UPSILON WITH TONOS */
        { "uring", 0x016F }, /* LATIN SMALL LETTER U WITH RING ABOVE */
        { "utilde", 0x0169 }, /* LATIN SMALL LETTER U WITH TILDE */
        { "v", 0x0076 }, /* LATIN SMALL LETTER V */
        { "w", 0x0077 }, /* LATIN SMALL LETTER W */
        { "wacute", 0x1E83 }, /* LATIN SMALL LETTER W WITH ACUTE */
        { "wcircumflex", 0x0175 }, /* LATIN SMALL LETTER W WITH CIRCUMFLEX */
        { "wdieresis", 0x1E85 }, /* LATIN SMALL LETTER W WITH DIAERESIS */
        { "weierstrass", 0x2118 }, /* SCRIPT CAPITAL P */
        { "wgrave", 0x1E81 }, /* LATIN SMALL LETTER W WITH GRAVE */
        { "x", 0x0078 }, /* LATIN SMALL LETTER X */
        { "xi", 0x03BE }, /* GREEK SMALL LETTER XI */
        { "y", 0x0079 }, /* LATIN SMALL LETTER Y */
        { "yacute", 0x00FD }, /* LATIN SMALL LETTER Y WITH ACUTE */
        { "ycircumflex", 0x0177 }, /* LATIN SMALL LETTER Y WITH CIRCUMFLEX */
        { "ydieresis", 0x00FF }, /* LATIN SMALL LETTER Y WITH DIAERESIS */
        { "yen", 0x00A5 }, /* YEN SIGN */
        { "ygrave", 0x1EF3 }, /* LATIN SMALL LETTER Y WITH GRAVE */
        { "z", 0x007A }, /* LATIN SMALL LETTER Z */
        { "zacute", 0x017A }, /* LATIN SMALL LETTER Z WITH ACUTE */
        { "zcaron", 0x017E }, /* LATIN SMALL LETTER Z WITH CARON */
        { "zdotaccent", 0x017C }, /* LATIN SMALL LETTER Z WITH DOT ABOVE */
        { "zero", 0x0030 }, /* DIGIT ZERO */
        { "zeroinferior", 0x2080 }, /* SUBSCRIPT ZERO */
        { "zerooldstyle", 0xF730 }, /* OLDSTYLE DIGIT ZERO */
        { "zerosuperior", 0x2070 }, /* SUPERSCRIPT ZERO */
        { "zeta", 0x03B6 } /* GREEK SMALL LETTER ZETA */
};

#endif /* ADOBETBL_H */
