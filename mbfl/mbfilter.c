/*
 * charset=UTF-8
 * vim600: encoding=utf-8
 */

/*
 * "streamable kanji code filter and converter"
 *
 * Copyright (c) 1998,1999,2000,2001 HappySize, Inc. All rights reserved.
 *
 * This software is released under the GNU Lesser General Public License.
 * (Version 2.1, February 1999)
 * Please read the following detail of the licence (in japanese).
 *
 * ◆使用許諾条件◆
 *
 * このソフトウェアは株式会社ハッピーサイズによって開発されました。株式会社ハッ
 * ピーサイズは、著作権法および万国著作権条約の定めにより、このソフトウェアに関
 * するすべての権利を留保する権利を持ち、ここに行使します。株式会社ハッピーサイ
 * ズは以下に明記した条件に従って、このソフトウェアを使用する排他的ではない権利
 * をお客様に許諾します。何人たりとも、以下の条件に反してこのソフトウェアを使用
 * することはできません。
 *
 * このソフトウェアを「GNU Lesser General Public License (Version 2.1, February
 * 1999)」に示された条件で使用することを、全ての方に許諾します。「GNU Lesser
 * General Public License」を満たさない使用には、株式会社ハッピーサイズから書面
 * による許諾を得る必要があります。
 *
 * 「GNU Lesser General Public License」の全文は以下のウェブページから取得でき
 * ます。「GNU Lesser General Public License」とは、これまでLibrary General
 * Public Licenseと呼ばれていたものです。
 *     http://www.gnu.org/ --- GNUウェブサイト
 *     http://www.gnu.org/copyleft/lesser.html --- ライセンス文面
 * このライセンスの内容がわからない方、守れない方には使用を許諾しません。
 *
 * しかしながら、当社とGNUプロジェクトとの特定の関係を示唆または主張するもので
 * はありません。
 *
 * ◆保証内容◆
 *
 * このソフトウェアは、期待された動作・機能・性能を持つことを目標として設計され
 * 開発されていますが、これを保証するものではありません。このソフトウェアは「こ
 * のまま」の状態で提供されており、たとえばこのソフトウェアの有用性ないし特定の
 * 目的に合致することといった、何らかの保証内容が、明示されたり暗黙に示されてい
 * る場合であっても、その保証は無効です。このソフトウェアを使用した結果ないし使
 * 用しなかった結果によって、直接あるいは間接に受けた身体的な傷害、財産上の損害
 * 、データの損失あるいはその他の全ての損害については、その損害の可能性が使用者
 * 、当社あるいは第三者によって警告されていた場合であっても、当社はその損害の賠
 * 償および補填を行いません。この規定は他の全ての、書面上または書面に無い保証・
 * 契約・規定に優先します。
 *
 * ◆著作権者の連絡先および使用条件についての問い合わせ先◆
 *
 * 〒102-0073
 * 東京都千代田区九段北1-13-5日本地所第一ビル4F
 * 株式会社ハッピーサイズ
 * Phone: 03-3512-3655, Fax: 03-3512-3656
 * Email: sales@happysize.co.jp
 * Web: http://happysize.com/
 *
 * ◆著者◆
 *
 * 金本　茂 <sgk@happysize.co.jp>
 *
 * ◆履歴◆
 *
 * 1998/11/10 sgk implementation in C++
 * 1999/4/25  sgk Cで書きなおし。
 * 1999/4/26  sgk 入力フィルタを実装。漢字コードを推定しながらフィルタを追加。
 * 1999/6/??      Unicodeサポート。
 * 1999/6/22  sgk ライセンスをLGPLに変更。
 *
 */

/* 
 * Unicode support
 *
 * Portions copyright (c) 1999,2000,2001 by the PHP3 internationalization team.
 * All rights reserved.
 *
 */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include "mbfilter.h"

#include "filters/mbfilter_euc_cn.h"
#include "filters/mbfilter_hz.h"
#include "filters/mbfilter_euc_tw.h"
#include "filters/mbfilter_big5.h"
#include "filters/mbfilter_uhc.h"
#include "filters/mbfilter_euc_kr.h"
#include "filters/mbfilter_iso2022_kr.h"
#include "filters/mbfilter_sjis.h"
#include "filters/mbfilter_jis.h"
#include "filters/mbfilter_euc_jp.h"
#include "filters/mbfilter_euc_jp_win.h"
#include "filters/mbfilter_ascii.h"
#include "filters/mbfilter_koi8r.h"
#include "filters/mbfilter_cp866.h"
#include "filters/mbfilter_cp932.h"
#include "filters/mbfilter_cp936.h"
#include "filters/mbfilter_cp1251.h"
#include "filters/mbfilter_cp1252.h"
#include "filters/mbfilter_iso8859_1.h"
#include "filters/mbfilter_iso8859_2.h"
#include "filters/mbfilter_iso8859_3.h"
#include "filters/mbfilter_iso8859_4.h"
#include "filters/mbfilter_iso8859_5.h"
#include "filters/mbfilter_iso8859_6.h"
#include "filters/mbfilter_iso8859_7.h"
#include "filters/mbfilter_iso8859_8.h"
#include "filters/mbfilter_iso8859_9.h"
#include "filters/mbfilter_iso8859_10.h"
#include "filters/mbfilter_iso8859_13.h"
#include "filters/mbfilter_iso8859_14.h"
#include "filters/mbfilter_iso8859_15.h"
#include "filters/mbfilter_base64.h"
#include "filters/mbfilter_qprint.h"
#include "filters/mbfilter_uuencode.h"
#include "filters/mbfilter_pass.h"
#include "filters/mbfilter_7bit.h"
#include "filters/mbfilter_utf7.h"
#include "filters/mbfilter_utf7imap.h"
#include "filters/mbfilter_utf8.h"
#include "filters/mbfilter_utf16.h"
#include "filters/mbfilter_byte2.h"
#include "filters/mbfilter_byte4.h"
#include "filters/mbfilter_ucs4.h"
#include "filters/mbfilter_ucs2.h"
#include "filters/mbfilter_htmlnumeric.h"

#include "nls/nls_ja.h"
#include "nls/nls_kr.h"
#include "nls/nls_zh.h"
#include "nls/nls_uni.h"
#include "nls/nls_de.h"
#include "nls/nls_ru.h"
#include "nls/nls_en.h"

#define	mbfl_malloc		malloc
#define	mbfl_realloc	realloc
#define	mbfl_calloc		calloc
#define	mbfl_free		free

#define	mbfl_pmalloc	malloc
#define	mbfl_prealloc	realloc
#define	mbfl_pfree		free

static const mbfl_language *mbfl_language_ptr_table[] = {
	&mbfl_language_uni,
	&mbfl_language_japanese,
	&mbfl_language_korean,
	&mbfl_language_simplified_chinese,
	&mbfl_language_traditional_chinese,
	&mbfl_language_english,
	&mbfl_language_german,
	&mbfl_language_russian,
	NULL
};

/* encoding byte width table */

/* encoding structure */
static const char *mbfl_encoding_auto_aliases[] = {"unknown", NULL};

static const mbfl_encoding mbfl_encoding_auto = {
	mbfl_no_encoding_auto,
	"auto",
	NULL,
	(const char *(*)[])&mbfl_encoding_auto_aliases,
	NULL,
	0
};

static const mbfl_encoding mbfl_encoding_wchar = {
	mbfl_no_encoding_wchar,
	"wchar",
	NULL,
	NULL,
	NULL,
	MBFL_ENCTYPE_WCS4BE
};

static const char *mbfl_encoding_html_ent_aliases[] = {"HTML", "html", NULL};

static const mbfl_encoding mbfl_encoding_html_ent = {
	mbfl_no_encoding_html_ent,
	"HTML-ENTITIES",
	"US-ASCII",
	(const char *(*)[])&mbfl_encoding_html_ent_aliases,
	NULL, /* mblen_table_html, Do not use table instead calulate length based on entities actually used */
	MBFL_ENCTYPE_HTML_ENT
};

static const mbfl_encoding mbfl_encoding_8bit = {
	mbfl_no_encoding_8bit,
	"8bit",
	"8bit",
	NULL,
	NULL,
	MBFL_ENCTYPE_SBCS
};

static const char *mbfl_encoding_utf32_aliases[] = {"utf32", NULL};

static const mbfl_encoding mbfl_encoding_utf32 = {
	mbfl_no_encoding_utf32,
	"UTF-32",
	"UTF-32",
	(const char *(*)[])&mbfl_encoding_utf32_aliases,
	NULL,
	MBFL_ENCTYPE_WCS4BE
};

static const mbfl_encoding mbfl_encoding_utf32be = {
	mbfl_no_encoding_utf32be,
	"UTF-32BE",
	"UTF-32BE",
	NULL,
	NULL,
	MBFL_ENCTYPE_WCS4BE
};

static const mbfl_encoding mbfl_encoding_utf32le = {
	mbfl_no_encoding_utf32le,
	"UTF-32LE",
	"UTF-32LE",
	NULL,
	NULL,
	MBFL_ENCTYPE_WCS4LE
};

static const mbfl_encoding *mbfl_encoding_ptr_list[] = {
	&mbfl_encoding_pass,
	&mbfl_encoding_auto,
	&mbfl_encoding_wchar,
	&mbfl_encoding_byte2be,
	&mbfl_encoding_byte2le,
	&mbfl_encoding_byte4be,
	&mbfl_encoding_byte4le,
	&mbfl_encoding_base64,
	&mbfl_encoding_uuencode,
	&mbfl_encoding_html_ent,
	&mbfl_encoding_qprint,
	&mbfl_encoding_7bit,
	&mbfl_encoding_8bit,
	&mbfl_encoding_ucs4,
	&mbfl_encoding_ucs4be,
	&mbfl_encoding_ucs4le,
	&mbfl_encoding_ucs2,
	&mbfl_encoding_ucs2be,
	&mbfl_encoding_ucs2le,
	&mbfl_encoding_utf32,
	&mbfl_encoding_utf32be,
	&mbfl_encoding_utf32le,
	&mbfl_encoding_utf16,
	&mbfl_encoding_utf16be,
	&mbfl_encoding_utf16le,
	&mbfl_encoding_utf8,
	&mbfl_encoding_utf7,
	&mbfl_encoding_utf7imap,
	&mbfl_encoding_ascii,
	&mbfl_encoding_euc_jp,
	&mbfl_encoding_sjis,
	&mbfl_encoding_eucjp_win,
	&mbfl_encoding_sjis_win,
	&mbfl_encoding_jis,
	&mbfl_encoding_2022jp,
	&mbfl_encoding_cp1252,
	&mbfl_encoding_8859_1,
	&mbfl_encoding_8859_2,
	&mbfl_encoding_8859_3,
	&mbfl_encoding_8859_4,
	&mbfl_encoding_8859_5,
	&mbfl_encoding_8859_6,
	&mbfl_encoding_8859_7,
	&mbfl_encoding_8859_8,
	&mbfl_encoding_8859_9,
	&mbfl_encoding_8859_10,
	&mbfl_encoding_8859_13,
	&mbfl_encoding_8859_14,
	&mbfl_encoding_8859_15,
	&mbfl_encoding_euc_cn,
	&mbfl_encoding_cp936,
	&mbfl_encoding_hz,
	&mbfl_encoding_euc_tw,
	&mbfl_encoding_big5,
	&mbfl_encoding_euc_kr,
	&mbfl_encoding_uhc,
	&mbfl_encoding_2022kr,
	&mbfl_encoding_cp1251,
	&mbfl_encoding_cp866,
	&mbfl_encoding_koi8r,
	NULL
};

/* hex character table "0123456789ABCDEF" */
static char mbfl_hexchar_table[] = {
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x41,0x42,0x43,0x44,0x45,0x46
};

/* forward */
static void mbfl_filt_conv_common_ctor(mbfl_convert_filter *filter);
static int mbfl_filt_conv_common_flush(mbfl_convert_filter *filter);
static void mbfl_filt_conv_common_dtor(mbfl_convert_filter *filter);

static void mbfl_filt_ident_common_ctor(mbfl_identify_filter *filter);
static void mbfl_filt_ident_common_dtor(mbfl_identify_filter *filter);
static void mbfl_filt_ident_false_ctor(mbfl_identify_filter *filter);
static int mbfl_filt_ident_utf8(int c, mbfl_identify_filter *filter);
static int mbfl_filt_ident_utf7(int c, mbfl_identify_filter *filter);
static int mbfl_filt_ident_ascii(int c, mbfl_identify_filter *filter);

static int mbfl_filt_ident_eucjp(int c, mbfl_identify_filter *filter);
static int mbfl_filt_ident_sjis(int c, mbfl_identify_filter *filter);
static int mbfl_filt_ident_sjiswin(int c, mbfl_identify_filter *filter);
static int mbfl_filt_ident_jis(int c, mbfl_identify_filter *filter);
static int mbfl_filt_ident_2022jp(int c, mbfl_identify_filter *filter);

static int mbfl_filt_ident_euccn(int c, mbfl_identify_filter *filter);
static int mbfl_filt_ident_cp936(int c, mbfl_identify_filter *filter);
static int mbfl_filt_ident_hz(int c, mbfl_identify_filter *filter);

static int mbfl_filt_ident_euctw(int c, mbfl_identify_filter *filter);
static int mbfl_filt_ident_big5(int c, mbfl_identify_filter *filter);

static int mbfl_filt_ident_euckr(int c, mbfl_identify_filter *filter);
static int mbfl_filt_ident_uhc(int c, mbfl_identify_filter *filter);
static int mbfl_filt_ident_2022kr(int c, mbfl_identify_filter *filter);

static int mbfl_filt_ident_cp1251(int c, mbfl_identify_filter *filter);
static int mbfl_filt_ident_cp866(int c, mbfl_identify_filter *filter);
static int mbfl_filt_ident_koi8r(int c, mbfl_identify_filter *filter);

static int mbfl_filt_ident_cp1252(int c, mbfl_identify_filter *filter);
static int mbfl_filt_ident_false(int c, mbfl_identify_filter *filter);
static int mbfl_filt_ident_true(int c, mbfl_identify_filter *filter);

/* convert filter function table */
static const struct mbfl_convert_vtbl vtbl_pass = {
	mbfl_no_encoding_pass,
	mbfl_no_encoding_pass,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_pass,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_byte2be_wchar = {
	mbfl_no_encoding_byte2be,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_byte2be_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_byte2be = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_byte2be,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_byte2be,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_byte2le_wchar = {
	mbfl_no_encoding_byte2le,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_byte2le_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_byte2le = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_byte2le,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_byte2le,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_byte4be_wchar = {
	mbfl_no_encoding_byte4be,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_byte4be_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_byte4be = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_byte4be,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_byte4be,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_byte4le_wchar = {
	mbfl_no_encoding_byte4le,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_byte4le_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_byte4le = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_byte4le,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_byte4le,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_8bit_b64 = {
	mbfl_no_encoding_8bit,
	mbfl_no_encoding_base64,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_base64enc,
	mbfl_filt_conv_base64enc_flush };

static const struct mbfl_convert_vtbl vtbl_b64_8bit = {
	mbfl_no_encoding_base64,
	mbfl_no_encoding_8bit,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_base64dec,
	mbfl_filt_conv_base64dec_flush };

static const struct mbfl_convert_vtbl vtbl_uuencode_8bit = {
	mbfl_no_encoding_uuencode,
	mbfl_no_encoding_8bit,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_uudec,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_html = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_html_ent,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_html_enc,
	mbfl_filt_conv_html_enc_flush };

static const struct mbfl_convert_vtbl vtbl_html_wchar = {
	mbfl_no_encoding_html_ent,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_html_dec_ctor,
	mbfl_filt_conv_html_dec_dtor,
	mbfl_filt_conv_html_dec,
	mbfl_filt_conv_html_dec_flush };

static const struct mbfl_convert_vtbl vtbl_8bit_qprint = {
	mbfl_no_encoding_8bit,
	mbfl_no_encoding_qprint,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_qprintenc,
	mbfl_filt_conv_qprintenc_flush };

static const struct mbfl_convert_vtbl vtbl_qprint_8bit = {
	mbfl_no_encoding_qprint,
	mbfl_no_encoding_8bit,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_qprintdec,
	mbfl_filt_conv_qprintdec_flush };

static const struct mbfl_convert_vtbl vtbl_8bit_7bit = {
	mbfl_no_encoding_8bit,
	mbfl_no_encoding_7bit,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_any_7bit,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_7bit_8bit = {
	mbfl_no_encoding_7bit,
	mbfl_no_encoding_8bit,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_pass,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_ucs4_wchar = {
	mbfl_no_encoding_ucs4,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_ucs4_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_ucs4 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_ucs4,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_ucs4be,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_ucs4be_wchar = {
	mbfl_no_encoding_ucs4be,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_byte4be_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_ucs4be = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_ucs4be,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_ucs4be,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_ucs4le_wchar = {
	mbfl_no_encoding_ucs4le,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_byte4le_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_ucs4le = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_ucs4le,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_ucs4le,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_ucs2_wchar = {
	mbfl_no_encoding_ucs2,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_ucs2_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_ucs2 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_ucs2,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_ucs2be,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_ucs2be_wchar = {
	mbfl_no_encoding_ucs2be,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_byte2be_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_ucs2be = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_ucs2be,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_ucs2be,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_ucs2le_wchar = {
	mbfl_no_encoding_ucs2le,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_byte2le_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_ucs2le = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_ucs2le,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_ucs2le,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_utf32_wchar = {
	mbfl_no_encoding_utf32,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_ucs4_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_utf32 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_utf32,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_ucs4be,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_utf32be_wchar = {
	mbfl_no_encoding_utf32be,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_byte4be_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_utf32be = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_utf32be,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_ucs4be,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_utf32le_wchar = {
	mbfl_no_encoding_utf32le,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_byte4le_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_utf32le = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_utf32le,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_ucs4le,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_utf16_wchar = {
	mbfl_no_encoding_utf16,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_utf16_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_utf16 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_utf16,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_utf16be,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_utf16be_wchar = {
	mbfl_no_encoding_utf16be,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_utf16be_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_utf16be = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_utf16be,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_utf16be,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_utf16le_wchar = {
	mbfl_no_encoding_utf16le,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_utf16le_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_utf16le = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_utf16le,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_utf16le,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_utf8_wchar = {
	mbfl_no_encoding_utf8,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_utf8_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_utf8 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_utf8,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_utf8,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_utf7_wchar = {
	mbfl_no_encoding_utf7,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_utf7_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_utf7 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_utf7,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_utf7,
	mbfl_filt_conv_wchar_utf7_flush };

static const struct mbfl_convert_vtbl vtbl_utf7imap_wchar = {
	mbfl_no_encoding_utf7imap,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_utf7imap_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_utf7imap = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_utf7imap,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_utf7imap,
	mbfl_filt_conv_wchar_utf7imap_flush };

static const struct mbfl_convert_vtbl vtbl_ascii_wchar = {
	mbfl_no_encoding_ascii,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_pass,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_ascii = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_ascii,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_ascii,
	mbfl_filt_conv_common_flush };

#if defined(HAVE_MBSTR_JA)
static const struct mbfl_convert_vtbl vtbl_eucjp_wchar = {
	mbfl_no_encoding_euc_jp,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_eucjp_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_eucjp = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_euc_jp,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_eucjp,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_sjis_wchar = {
	mbfl_no_encoding_sjis,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_sjis_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_sjis = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_sjis,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_sjis,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_jis_wchar = {
	mbfl_no_encoding_jis,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_jis_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_jis = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_jis,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_jis,
	mbfl_filt_conv_any_jis_flush };

static const struct mbfl_convert_vtbl vtbl_2022jp_wchar = {
	mbfl_no_encoding_2022jp,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_jis_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_2022jp = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_2022jp,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_2022jp,
	mbfl_filt_conv_any_jis_flush };

static const struct mbfl_convert_vtbl vtbl_eucjpwin_wchar = {
	mbfl_no_encoding_eucjp_win,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_eucjpwin_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_eucjpwin = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_eucjp_win,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_eucjpwin,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_sjiswin_wchar = {
	mbfl_no_encoding_sjis_win,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_sjiswin_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_sjiswin = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_sjis_win,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_sjiswin,
	mbfl_filt_conv_common_flush };
#endif /* HAVE_MBSTR_JA */

#if defined(HAVE_MBSTR_CN)
static const struct mbfl_convert_vtbl vtbl_euccn_wchar = {
	mbfl_no_encoding_euc_cn,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_euccn_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_euccn = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_euc_cn,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_euccn,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_cp936_wchar = {
	mbfl_no_encoding_cp936,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_cp936_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_cp936 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_cp936,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_cp936,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_hz_wchar = {
	mbfl_no_encoding_hz,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_hz_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_hz = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_hz,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_hz,
	mbfl_filt_conv_any_hz_flush };

#endif /* HAVE_MBSTR_CN */

#if defined(HAVE_MBSTR_TW)
static const struct mbfl_convert_vtbl vtbl_euctw_wchar = {
	mbfl_no_encoding_euc_tw,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_euctw_wchar,
	mbfl_filt_conv_common_flush };

static struct mbfl_convert_vtbl vtbl_wchar_euctw = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_euc_tw,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_euctw,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_big5_wchar = {
	mbfl_no_encoding_big5,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_big5_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_big5 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_big5,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_big5,
	mbfl_filt_conv_common_flush };
#endif /* HAVE_MBSTR_TW */

#if defined(HAVE_MBSTR_KR)
static const struct mbfl_convert_vtbl vtbl_euckr_wchar = {
	mbfl_no_encoding_euc_kr,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_euckr_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_euckr = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_euc_kr,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_euckr,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_uhc_wchar = {
	mbfl_no_encoding_uhc,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_uhc_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_uhc = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_uhc,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_uhc,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_2022kr = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_2022kr,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_2022kr,
	mbfl_filt_conv_any_2022kr_flush };

static const struct mbfl_convert_vtbl vtbl_2022kr_wchar = {
	mbfl_no_encoding_2022kr,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_2022kr_wchar,
	mbfl_filt_conv_common_flush };
#endif /* HAVE_MBSTR_KR */

#if defined(HAVE_MBSTR_RU)
static const struct mbfl_convert_vtbl vtbl_wchar_cp1251 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_cp1251,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_cp1251,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_cp1251_wchar = {
	mbfl_no_encoding_cp1251,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_cp1251_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_cp866 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_cp866,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_cp866,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_cp866_wchar = {
	mbfl_no_encoding_cp866,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_cp866_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_koi8r = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_koi8r,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_koi8r,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_koi8r_wchar = {
	mbfl_no_encoding_koi8r,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_koi8r_wchar,
	mbfl_filt_conv_common_flush };
#endif /* HAVE_MBSTR_RU */

static const struct mbfl_convert_vtbl vtbl_cp1252_wchar = {
	mbfl_no_encoding_cp1252,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_cp1252_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_cp1252 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_cp1252,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_cp1252,
	mbfl_filt_conv_common_flush };


static const struct mbfl_convert_vtbl vtbl_8859_1_wchar = {
	mbfl_no_encoding_8859_1,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_pass,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_8859_1 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_8859_1,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_8859_1,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_8859_2_wchar = {
	mbfl_no_encoding_8859_2,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_8859_2_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_8859_2 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_8859_2,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_8859_2,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_8859_3_wchar = {
	mbfl_no_encoding_8859_3,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_8859_3_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_8859_3 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_8859_3,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_8859_3,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_8859_4_wchar = {
	mbfl_no_encoding_8859_4,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_8859_4_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_8859_4 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_8859_4,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_8859_4,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_8859_5_wchar = {
	mbfl_no_encoding_8859_5,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_8859_5_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_8859_5 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_8859_5,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_8859_5,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_8859_6_wchar = {
	mbfl_no_encoding_8859_6,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_8859_6_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_8859_6 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_8859_6,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_8859_6,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_8859_7_wchar = {
	mbfl_no_encoding_8859_7,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_8859_7_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_8859_7 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_8859_7,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_8859_7,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_8859_8_wchar = {
	mbfl_no_encoding_8859_8,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_8859_8_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_8859_8 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_8859_8,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_8859_8,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_8859_9_wchar = {
	mbfl_no_encoding_8859_9,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_8859_9_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_8859_9 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_8859_9,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_8859_9,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_8859_10_wchar = {
	mbfl_no_encoding_8859_10,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_8859_10_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_8859_10 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_8859_10,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_8859_10,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_8859_13_wchar = {
	mbfl_no_encoding_8859_13,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_8859_13_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_8859_13 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_8859_13,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_8859_13,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_8859_14_wchar = {
	mbfl_no_encoding_8859_14,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_8859_14_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_8859_14 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_8859_14,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_8859_14,
	mbfl_filt_conv_common_flush };

static struct mbfl_convert_vtbl vtbl_8859_15_wchar = {
	mbfl_no_encoding_8859_15,
	mbfl_no_encoding_wchar,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_8859_15_wchar,
	mbfl_filt_conv_common_flush };

static const struct mbfl_convert_vtbl vtbl_wchar_8859_15 = {
	mbfl_no_encoding_wchar,
	mbfl_no_encoding_8859_15,
	mbfl_filt_conv_common_ctor,
	mbfl_filt_conv_common_dtor,
	mbfl_filt_conv_wchar_8859_15,
	mbfl_filt_conv_common_flush };


static const struct mbfl_convert_vtbl *mbfl_convert_filter_list[] = {
	&vtbl_utf8_wchar,
	&vtbl_wchar_utf8,
#if defined(HAVE_MBSTR_JA)
	&vtbl_eucjp_wchar,
	&vtbl_wchar_eucjp,
	&vtbl_sjis_wchar,
	&vtbl_wchar_sjis,
	&vtbl_jis_wchar,
	&vtbl_wchar_jis,
	&vtbl_2022jp_wchar,
	&vtbl_wchar_2022jp,
	&vtbl_eucjpwin_wchar,
	&vtbl_wchar_eucjpwin,
	&vtbl_sjiswin_wchar,
	&vtbl_wchar_sjiswin,
#endif
#if defined(HAVE_MBSTR_CN)
	&vtbl_euccn_wchar,
	&vtbl_wchar_euccn,
	&vtbl_cp936_wchar,
	&vtbl_wchar_cp936,
	&vtbl_hz_wchar,
	&vtbl_wchar_hz,
#endif
#if defined(HAVE_MBSTR_TW)
	&vtbl_euctw_wchar,
	&vtbl_wchar_euctw,
	&vtbl_big5_wchar,
	&vtbl_wchar_big5,
#endif
#if defined(HAVE_MBSTR_KR)
	&vtbl_euckr_wchar,
	&vtbl_wchar_euckr,
	&vtbl_uhc_wchar,
	&vtbl_wchar_uhc,
	&vtbl_2022kr_wchar,
	&vtbl_wchar_2022kr,
#endif
#if defined(HAVE_MBSTR_RU)
	&vtbl_cp1251_wchar,
	&vtbl_wchar_cp1251,
	&vtbl_cp866_wchar,
	&vtbl_wchar_cp866,
	&vtbl_koi8r_wchar,
	&vtbl_wchar_koi8r,
#endif
	&vtbl_cp1252_wchar,
	&vtbl_wchar_cp1252,
	&vtbl_ascii_wchar,
	&vtbl_wchar_ascii,
	&vtbl_8859_1_wchar,
	&vtbl_wchar_8859_1,
	&vtbl_8859_2_wchar,
	&vtbl_wchar_8859_2,
	&vtbl_8859_3_wchar,
	&vtbl_wchar_8859_3,
	&vtbl_8859_4_wchar,
	&vtbl_wchar_8859_4,
	&vtbl_8859_5_wchar,
	&vtbl_wchar_8859_5,
	&vtbl_8859_6_wchar,
	&vtbl_wchar_8859_6,
	&vtbl_8859_7_wchar,
	&vtbl_wchar_8859_7,
	&vtbl_8859_8_wchar,
	&vtbl_wchar_8859_8,
	&vtbl_8859_9_wchar,
	&vtbl_wchar_8859_9,
	&vtbl_8859_10_wchar,
	&vtbl_wchar_8859_10,
	&vtbl_8859_13_wchar,
	&vtbl_wchar_8859_13,
	&vtbl_8859_14_wchar,
	&vtbl_wchar_8859_14,
	&vtbl_8859_15_wchar,
	&vtbl_wchar_8859_15,
	&vtbl_8bit_b64,
	&vtbl_b64_8bit,
	&vtbl_uuencode_8bit,
	&vtbl_wchar_html,
	&vtbl_html_wchar,
	&vtbl_8bit_qprint,
	&vtbl_qprint_8bit,
	&vtbl_8bit_7bit,
	&vtbl_7bit_8bit,
	&vtbl_utf7_wchar,
	&vtbl_wchar_utf7,
	&vtbl_utf7imap_wchar,
	&vtbl_wchar_utf7imap,
	&vtbl_utf16_wchar,
	&vtbl_wchar_utf16,
	&vtbl_utf16be_wchar,
	&vtbl_wchar_utf16be,
	&vtbl_utf16le_wchar,
	&vtbl_wchar_utf16le,
	&vtbl_utf32_wchar,
	&vtbl_wchar_utf32,
	&vtbl_utf32be_wchar,
	&vtbl_wchar_utf32be,
	&vtbl_utf32le_wchar,
	&vtbl_wchar_utf32le,
	&vtbl_ucs4_wchar,
	&vtbl_wchar_ucs4,
	&vtbl_ucs4be_wchar,
	&vtbl_wchar_ucs4be,
	&vtbl_ucs4le_wchar,
	&vtbl_wchar_ucs4le,
	&vtbl_ucs2_wchar,
	&vtbl_wchar_ucs2,
	&vtbl_ucs2be_wchar,
	&vtbl_wchar_ucs2be,
	&vtbl_ucs2le_wchar,
	&vtbl_wchar_ucs2le,
	&vtbl_byte4be_wchar,
	&vtbl_wchar_byte4be,
	&vtbl_byte4le_wchar,
	&vtbl_wchar_byte4le,
	&vtbl_byte2be_wchar,
	&vtbl_wchar_byte2be,
	&vtbl_byte2le_wchar,
	&vtbl_wchar_byte2le,
	&vtbl_pass,
	NULL
};


/* identify filter function table */
static const struct mbfl_identify_vtbl vtbl_identify_ascii = {
	mbfl_no_encoding_ascii,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_ascii };

static const struct mbfl_identify_vtbl vtbl_identify_utf8 = {
	mbfl_no_encoding_utf8,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_utf8 };

static const struct mbfl_identify_vtbl vtbl_identify_utf7 = {
	mbfl_no_encoding_utf7,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_utf7 };

#if defined(HAVE_MBSTR_JA)
static const struct mbfl_identify_vtbl vtbl_identify_eucjp = {
	mbfl_no_encoding_euc_jp,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_eucjp };

static const struct mbfl_identify_vtbl vtbl_identify_eucjpwin = {
	mbfl_no_encoding_eucjp_win,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_eucjp };

static const struct mbfl_identify_vtbl vtbl_identify_sjis = {
	mbfl_no_encoding_sjis,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_sjis };

static const struct mbfl_identify_vtbl vtbl_identify_sjiswin = {
	mbfl_no_encoding_sjis_win,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_sjiswin };

static const struct mbfl_identify_vtbl vtbl_identify_jis = {
	mbfl_no_encoding_jis,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_jis };

static const struct mbfl_identify_vtbl vtbl_identify_2022jp = {
	mbfl_no_encoding_2022jp,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_2022jp };
#endif /* HAVE_MBSTR_JA */

#if defined(HAVE_MBSTR_CN)
static struct mbfl_identify_vtbl vtbl_identify_euccn = {
	mbfl_no_encoding_euc_cn,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_euccn };

static struct mbfl_identify_vtbl vtbl_identify_cp936 = {
	mbfl_no_encoding_cp936,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_cp936 };

static struct mbfl_identify_vtbl vtbl_identify_hz = {
	mbfl_no_encoding_hz,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_hz };

#endif /* HAVE_MBSTR_CN */

#if defined(HAVE_MBSTR_TW)
static struct mbfl_identify_vtbl vtbl_identify_euctw = {
	mbfl_no_encoding_euc_tw,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_euctw };

static struct mbfl_identify_vtbl vtbl_identify_big5 = {
	mbfl_no_encoding_big5,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_big5 };
#endif /* HAVE_MBSTR_TW */

#if defined(HAVE_MBSTR_KR)
static struct mbfl_identify_vtbl vtbl_identify_euckr = {
	mbfl_no_encoding_euc_kr,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_euckr };

static struct mbfl_identify_vtbl vtbl_identify_uhc = {
	mbfl_no_encoding_uhc,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_uhc };

static struct mbfl_identify_vtbl vtbl_identify_2022kr = {
	mbfl_no_encoding_2022kr,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_2022kr };

#endif /* HAVE_MBSTR_KR */

#if defined(HAVE_MBSTR_RU)
static struct mbfl_identify_vtbl vtbl_identify_cp1251 = {
	mbfl_no_encoding_cp1251,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_cp1251 };

static struct mbfl_identify_vtbl vtbl_identify_cp866 = {
	mbfl_no_encoding_cp866,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_cp866 };

static struct mbfl_identify_vtbl vtbl_identify_koi8r = {
	mbfl_no_encoding_koi8r,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_koi8r };
#endif /* HAVE_MBSTR_RU */

static const struct mbfl_identify_vtbl vtbl_identify_cp1252 = {
	mbfl_no_encoding_cp1252,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_cp1252 };

static const struct mbfl_identify_vtbl vtbl_identify_8859_1 = {
	mbfl_no_encoding_8859_1,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_true };

static const struct mbfl_identify_vtbl vtbl_identify_8859_2 = {
	mbfl_no_encoding_8859_2,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_true };

static const struct mbfl_identify_vtbl vtbl_identify_8859_3 = {
	mbfl_no_encoding_8859_3,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_true };

static const struct mbfl_identify_vtbl vtbl_identify_8859_4 = {
	mbfl_no_encoding_8859_4,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_true };

static const struct mbfl_identify_vtbl vtbl_identify_8859_5 = {
	mbfl_no_encoding_8859_5,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_true };

static const struct mbfl_identify_vtbl vtbl_identify_8859_6 = {
	mbfl_no_encoding_8859_6,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_true };

static const struct mbfl_identify_vtbl vtbl_identify_8859_7 = {
	mbfl_no_encoding_8859_7,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_true };

static const struct mbfl_identify_vtbl vtbl_identify_8859_8 = {
	mbfl_no_encoding_8859_8,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_true };

static const struct mbfl_identify_vtbl vtbl_identify_8859_9 = {
	mbfl_no_encoding_8859_9,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_true };

static const struct mbfl_identify_vtbl vtbl_identify_8859_10 = {
	mbfl_no_encoding_8859_10,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_true };

static const struct mbfl_identify_vtbl vtbl_identify_8859_13 = {
	mbfl_no_encoding_8859_13,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_true };

static const struct mbfl_identify_vtbl vtbl_identify_8859_14 = {
	mbfl_no_encoding_8859_14,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_true };

static const struct mbfl_identify_vtbl vtbl_identify_8859_15 = {
	mbfl_no_encoding_8859_15,
	mbfl_filt_ident_common_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_true };

static const struct mbfl_identify_vtbl vtbl_identify_false = {
	mbfl_no_encoding_pass,
	mbfl_filt_ident_false_ctor,
	mbfl_filt_ident_common_dtor,
	mbfl_filt_ident_false };

static const struct mbfl_identify_vtbl *mbfl_identify_filter_list[] = {
	&vtbl_identify_utf8,
	&vtbl_identify_utf7,
	&vtbl_identify_ascii,
#if defined(HAVE_MBSTR_JA)
	&vtbl_identify_eucjp,
	&vtbl_identify_sjis,
	&vtbl_identify_eucjpwin,
	&vtbl_identify_sjiswin,
	&vtbl_identify_jis,
	&vtbl_identify_2022jp,
#endif
#if defined(HAVE_MBSTR_CN)
	&vtbl_identify_euccn,
	&vtbl_identify_cp936,
	&vtbl_identify_hz,
#endif
#if defined(HAVE_MBSTR_TW)
	&vtbl_identify_euctw,
	&vtbl_identify_big5,
#endif
#if defined(HAVE_MBSTR_KR)
	&vtbl_identify_euckr,
	&vtbl_identify_uhc,
	&vtbl_identify_2022kr,
#endif
#if defined(HAVE_MBSTR_RU)
	&vtbl_identify_cp1251,
	&vtbl_identify_cp866,
	&vtbl_identify_koi8r,
#endif
	&vtbl_identify_cp1252,
	&vtbl_identify_8859_1,
	&vtbl_identify_8859_2,
	&vtbl_identify_8859_3,
	&vtbl_identify_8859_4,
	&vtbl_identify_8859_5,
	&vtbl_identify_8859_6,
	&vtbl_identify_8859_7,
	&vtbl_identify_8859_8,
	&vtbl_identify_8859_9,
	&vtbl_identify_8859_10,
	&vtbl_identify_8859_13,
	&vtbl_identify_8859_14,
	&vtbl_identify_8859_15,
	&vtbl_identify_false,
	NULL
};


/* language resolver */
const mbfl_language *
mbfl_name2language(const char *name)
{
	const mbfl_language *language;
	int i, j;

	if (name == NULL) {
		return NULL;
	}

	i = 0;
	while ((language = mbfl_language_ptr_table[i++]) != NULL){
		if (strcasecmp(language->name, name) == 0) {
			return language;
		}
	}

	i = 0;
	while ((language = mbfl_language_ptr_table[i++]) != NULL){
		if (strcasecmp(language->short_name, name) == 0) {
			return language;
		}
	}

	/* serch aliases */
	i = 0;
	while ((language = mbfl_language_ptr_table[i++]) != NULL) {
		if (language->aliases != NULL) {
			j = 0;
			while ((*language->aliases)[j] != NULL) {
				if (strcasecmp((*language->aliases)[j], name) == 0) {
					return language;
				}
				j++;
			}
		}
	}

	return NULL;
}

const mbfl_language *
mbfl_no2language(enum mbfl_no_language no_language)
{
	const mbfl_language *language;
	int i;

	i = 0;
	while ((language = mbfl_language_ptr_table[i++]) != NULL){
		if (language->no_language == no_language) {
			return language;
		}
	}

	return NULL;
}

enum mbfl_no_language
mbfl_name2no_language(const char *name)
{
	const mbfl_language *language;

	language = mbfl_name2language(name);
	if (language == NULL) {
		return mbfl_no_language_invalid;
	} else {
		return language->no_language;
	}
}

const char *
mbfl_no_language2name(enum mbfl_no_language no_language)
{
	const mbfl_language *language;

	language = mbfl_no2language(no_language);
	if (language == NULL) {
		return "";
	} else {
		return language->name;
	}
}



/* encoding resolver */
const mbfl_encoding *
mbfl_name2encoding(const char *name)
{
	const mbfl_encoding *encoding;
	int i, j;

	if (name == NULL) {
		return NULL;
	}

	i = 0;
	while ((encoding = mbfl_encoding_ptr_list[i++]) != NULL){
		if (strcasecmp(encoding->name, name) == 0) {
			return encoding;
		}
	}

	/* serch MIME charset name */
	i = 0;
	while ((encoding = mbfl_encoding_ptr_list[i++]) != NULL) {
		if (encoding->mime_name != NULL) {
			if (strcasecmp(encoding->mime_name, name) == 0) {
				return encoding;
			}
		}
	}

	/* serch aliases */
	i = 0;
	while ((encoding = mbfl_encoding_ptr_list[i++]) != NULL) {
		if (encoding->aliases != NULL) {
			j = 0;
			while ((*encoding->aliases)[j] != NULL) {
				if (strcasecmp((*encoding->aliases)[j], name) == 0) {
					return encoding;
				}
				j++;
			}
		}
	}

	return NULL;
}

const mbfl_encoding *
mbfl_no2encoding(enum mbfl_no_encoding no_encoding)
{
	const mbfl_encoding *encoding;
	int i;

	i = 0;
	while ((encoding = mbfl_encoding_ptr_list[i++]) != NULL){
		if (encoding->no_encoding == no_encoding) {
			return encoding;
		}
	}

	return NULL;
}

enum mbfl_no_encoding
mbfl_name2no_encoding(const char *name)
{
	const mbfl_encoding *encoding;

	encoding = mbfl_name2encoding(name);
	if (encoding == NULL) {
		return mbfl_no_encoding_invalid;
	} else {
		return encoding->no_encoding;
	}
}

const char *
mbfl_no_encoding2name(enum mbfl_no_encoding no_encoding)
{
	const mbfl_encoding *encoding;

	encoding = mbfl_no2encoding(no_encoding);
	if (encoding == NULL) {
		return "";
	} else {
		return encoding->name;
	}
}

const char *
mbfl_no2preferred_mime_name(enum mbfl_no_encoding no_encoding)
{
	const mbfl_encoding *encoding;

	encoding = mbfl_no2encoding(no_encoding);
	if (encoding != NULL && encoding->mime_name != NULL && encoding->mime_name[0] != '\0') {
		return encoding->mime_name;
	} else {
		return NULL;
	}
}

int
mbfl_is_support_encoding(const char *name)
{
	const mbfl_encoding *encoding;

	encoding = mbfl_name2encoding(name);
	if (encoding == NULL) {
		return 0;
	} else {
		return 1;
	}
}



/*
 * memory device output functions
 */
void
mbfl_memory_device_init(mbfl_memory_device *device, int initsz, int allocsz)
{
	if (device) {
		device->length = 0;
		device->buffer = (unsigned char *)0;
		if (initsz > 0) {
			device->buffer = (unsigned char *)mbfl_malloc(initsz*sizeof(unsigned char));
			if (device->buffer != NULL) {
				device->length = initsz;
			}
		}
		device->pos= 0;
		if (allocsz > MBFL_MEMORY_DEVICE_ALLOC_SIZE) {
			device->allocsz = allocsz;
		} else {
			device->allocsz = MBFL_MEMORY_DEVICE_ALLOC_SIZE;
		}
	}
}

void
mbfl_memory_device_realloc(mbfl_memory_device *device, int initsz, int allocsz)
{
	unsigned char *tmp;

	if (device) {
		if (initsz > device->length) {
			tmp = (unsigned char *)mbfl_realloc((void *)device->buffer, initsz*sizeof(unsigned char));
			if (tmp != NULL) {
				device->buffer = tmp;
				device->length = initsz;
			}
		}
		if (allocsz > MBFL_MEMORY_DEVICE_ALLOC_SIZE) {
			device->allocsz = allocsz;
		} else {
			device->allocsz = MBFL_MEMORY_DEVICE_ALLOC_SIZE;
		}
	}
}

void
mbfl_memory_device_clear(mbfl_memory_device *device)
{
	if (device) {
		if (device->buffer) {
			mbfl_free(device->buffer);
		}
		device->buffer = (unsigned char *)0;
		device->length = 0;
		device->pos = 0;
	}
}

void
mbfl_memory_device_reset(mbfl_memory_device *device)
{
	if (device) {
		device->pos = 0;
	}
}

void
mbfl_memory_device_unput(mbfl_memory_device *device)
{
	if (device->pos > 0) {
		device->pos--;
	}
}

mbfl_string *
mbfl_memory_device_result(mbfl_memory_device *device, mbfl_string *result)
{
	if (device && result) {
		result->len = device->pos;
		mbfl_memory_device_output4('\0', device);
		result->val = device->buffer;
		device->buffer = (unsigned char *)0;
		device->length = 0;
		device->pos= 0;
		if (result->val == NULL) {
			result->len = 0;
			result = NULL;
		}
	} else {
		result = NULL;
	}

	return result;
}

int
mbfl_memory_device_output(int c, void *data)
{
	mbfl_memory_device *device = (mbfl_memory_device *)data;

	if (device->pos >= device->length) {
		/* reallocate buffer */
		int newlen;
		unsigned char *tmp;

		newlen = device->length + device->allocsz;
		tmp = (unsigned char *)mbfl_realloc((void *)device->buffer, newlen*sizeof(unsigned char));
		if (tmp == NULL) {
			return -1;
		}
		device->length = newlen;
		device->buffer = tmp;
	}

	device->buffer[device->pos++] = (unsigned char)c;
	return c;
}

int
mbfl_memory_device_output2(int c, void *data)
{
	mbfl_memory_device *device = (mbfl_memory_device *)data;

	if ((device->pos + 2) >= device->length) {
		/* reallocate buffer */
		int newlen;
		unsigned char *tmp;

		newlen = device->length + device->allocsz;
		tmp = (unsigned char *)mbfl_realloc((void *)device->buffer, newlen*sizeof(unsigned char));
		if (tmp == NULL) {
			return -1;
		}
		device->length = newlen;
		device->buffer = tmp;
	}

	device->buffer[device->pos++] = (unsigned char)((c >> 8) & 0xff);
	device->buffer[device->pos++] = (unsigned char)(c & 0xff);

	return c;
}

int
mbfl_memory_device_output4(int c, void* data)
{
	mbfl_memory_device *device = (mbfl_memory_device *)data;

	if ((device->pos + 4) >= device->length) {
		/* reallocate buffer */
		int newlen;
		unsigned char *tmp;

		newlen = device->length + device->allocsz;
		tmp = (unsigned char *)mbfl_realloc((void *)device->buffer, newlen*sizeof(unsigned char));
		if (tmp == NULL) {
			return -1;
		}
		device->length = newlen;
		device->buffer = tmp;
	}

	device->buffer[device->pos++] = (unsigned char)((c >> 24) & 0xff);
	device->buffer[device->pos++] = (unsigned char)((c >> 16) & 0xff);
	device->buffer[device->pos++] = (unsigned char)((c >> 8) & 0xff);
	device->buffer[device->pos++] = (unsigned char)(c & 0xff);

	return c;
}

int
mbfl_memory_device_strcat(mbfl_memory_device *device, const char *psrc)
{
	int len;
	unsigned char *w;
	const unsigned char *p;

	len = 0;
	p = psrc;
	while (*p) {
		p++;
		len++;
	}

	if ((device->pos + len) >= device->length) {
		/* reallocate buffer */
		int newlen = device->length + (len + MBFL_MEMORY_DEVICE_ALLOC_SIZE)*sizeof(unsigned char);
		unsigned char *tmp = (unsigned char *)mbfl_realloc((void *)device->buffer, newlen*sizeof(unsigned char));
		if (tmp == NULL) {
			return -1;
		}
		device->length = newlen;
		device->buffer = tmp;
	}

	p = psrc;
	w = &device->buffer[device->pos];
	device->pos += len;
	while (len > 0) {
		*w++ = *p++;
		len--;
	}

	return len;
}

int
mbfl_memory_device_strncat(mbfl_memory_device *device, const char *psrc, int len)
{
	unsigned char *w;

	if ((device->pos + len) >= device->length) {
		/* reallocate buffer */
		int newlen = device->length + len + MBFL_MEMORY_DEVICE_ALLOC_SIZE;
		unsigned char *tmp = (unsigned char *)mbfl_realloc((void *)device->buffer, newlen*sizeof(unsigned char));
		if (tmp == NULL) {
			return -1;
		}
		device->length = newlen;
		device->buffer = tmp;
	}

	w = &device->buffer[device->pos];
	device->pos += len;
	while (len > 0) {
		*w++ = *psrc++;
		len--;
	}

	return len;
}

int
mbfl_memory_device_devcat(mbfl_memory_device *dest, mbfl_memory_device *src)
{
	int n;
	unsigned char *p, *w;

	if ((dest->pos + src->pos) >= dest->length) {
		/* reallocate buffer */
		int newlen = dest->length + src->pos + MBFL_MEMORY_DEVICE_ALLOC_SIZE;
		unsigned char *tmp = (unsigned char *)mbfl_realloc((void *)dest->buffer, newlen*sizeof(unsigned char));
		if (tmp == NULL) {
			return -1;
		}
		dest->length = newlen;
		dest->buffer = tmp;
	}

	p = src->buffer;
	w = &dest->buffer[dest->pos];
	n = src->pos;
	dest->pos += n;
	while (n > 0) {
		*w++ = *p++;
		n--;
	}

	return n;
}

void
mbfl_wchar_device_init(mbfl_wchar_device *device)
{
	if (device) {
		device->buffer = (unsigned int *)0;
		device->length = 0;
		device->pos= 0;
		device->allocsz = MBFL_MEMORY_DEVICE_ALLOC_SIZE;
	}
}

void
mbfl_wchar_device_clear(mbfl_wchar_device *device)
{
	if (device) {
		if (device->buffer) {
			mbfl_free(device->buffer);
		}
		device->buffer = (unsigned int*)0;
		device->length = 0;
		device->pos = 0;
	}
}

int
mbfl_wchar_device_output(int c, void *data)
{
	mbfl_wchar_device *device = (mbfl_wchar_device *)data;

	if (device->pos >= device->length) {
		/* reallocate buffer */
		int newlen;
		unsigned int *tmp;

		newlen = device->length + device->allocsz;
		tmp = (unsigned int *)mbfl_realloc((void *)device->buffer, newlen*sizeof(int));
		if (tmp == NULL) {
			return -1;
		}
		device->length = newlen;
		device->buffer = tmp;
	}

	device->buffer[device->pos++] = c;

	return c;
}


/*
 * string object
 */
void
mbfl_string_init(mbfl_string *string)
{
	if (string) {
		string->no_language = mbfl_no_language_uni;
		string->no_encoding = mbfl_no_encoding_pass;
		string->val = (unsigned char*)0;
		string->len = 0;
	}
}

void
mbfl_string_init_set(mbfl_string *string, enum mbfl_no_language no_language, enum mbfl_no_encoding no_encoding)
{
	if (string) {
		string->no_language = no_language;
		string->no_encoding = no_encoding;
		string->val = (unsigned char*)0;
		string->len = 0;
	}
}

void
mbfl_string_clear(mbfl_string *string)
{
	if (string) {
		if (string->val != (unsigned char*)0) {
			mbfl_free(string->val);
		}
		string->val = (unsigned char*)0;
		string->len = 0;
	}
}



/*
 * encoding filter
 */
#define CK(statement)	do { if ((statement) < 0) return (-1); } while (0)

/*
 * commonly used constructor and destructor
 */
static void
mbfl_filt_conv_common_ctor(mbfl_convert_filter *filter)
{
	filter->status = 0;
	filter->cache = 0;
}

static int
mbfl_filt_conv_common_flush(mbfl_convert_filter *filter)
{
	filter->status = 0;
	filter->cache = 0;
	return 0;
}

static void
mbfl_filt_conv_common_dtor(mbfl_convert_filter *filter)
{
	filter->status = 0;
	filter->cache = 0;
}

/*
 *
 * identify filter functions
 *
 */

static void
mbfl_filt_ident_common_ctor(mbfl_identify_filter *filter)
{
	filter->status = 0;
	filter->flag = 0;
}

static void
mbfl_filt_ident_common_dtor(mbfl_identify_filter *filter)
{
	filter->status = 0;
}

static int
mbfl_filt_ident_ascii(int c, mbfl_identify_filter *filter)
{
	if (c >= 0x20 && c < 0x80) {
		;
	} else if (c == 0x0d || c == 0x0a || c == 0x09 || c == 0) {	/* CR or LF or HTAB or null */
		;
	} else {
		filter->flag = 1;
	}

	return c;
}

static int
mbfl_filt_ident_utf8(int c, mbfl_identify_filter *filter)
{
	if (c < 0x80) {
		if (c < 0) { 
			filter->flag = 1;	/* bad */
		} else if (c != 0 && filter->status) {
			filter->flag = 1;	/* bad */
		}
		filter->status = 0;
	} else if (c < 0xc0) {
		switch (filter->status) {
		case 0x20: /* 3 byte code 2nd char */
		case 0x30: /* 4 byte code 2nd char */
		case 0x31: /* 4 byte code 3rd char */
		case 0x40: /* 5 byte code 2nd char */
		case 0x41: /* 5 byte code 3rd char */
		case 0x42: /* 5 byte code 4th char */
		case 0x50: /* 6 byte code 2nd char */
		case 0x51: /* 6 byte code 3rd char */
		case 0x52: /* 6 byte code 4th char */
		case 0x53: /* 6 byte code 5th char */
			filter->status++;
			break;
		case 0x10: /* 2 byte code 2nd char */
		case 0x21: /* 3 byte code 3rd char */
		case 0x32: /* 4 byte code 4th char */
		case 0x43: /* 5 byte code 5th char */
		case 0x54: /* 6 byte code 6th char */
			filter->status = 0;
			break;
		default:
			filter->flag = 1;	/* bad */
			filter->status = 0;
			break;
		}
	} else {
		if (filter->status) {
			filter->flag = 1;	/* bad */
		}
		filter->status = 0;
		if (c < 0xe0) {				/* 2 byte code first char */
			filter->status = 0x10;
		} else if (c < 0xf0) {		/* 3 byte code 1st char */
			filter->status = 0x20;
		} else if (c < 0xf8) {		/* 4 byte code 1st char */
			filter->status = 0x30;
		} else if (c < 0xfc) {		/* 5 byte code 1st char */
			filter->status = 0x40;
		} else if (c < 0xfe) {		/* 6 byte code 1st char */
			filter->status = 0x50;
		} else {
			filter->flag = 1;	/* bad */
		}
	}

	return c;
}

static int
mbfl_filt_ident_utf7(int c, mbfl_identify_filter *filter)
{
	int n;

	switch (filter->status) {
	/* directly encoded characters */
	case 0:
		if (c == 0x2b) {	/* '+'  shift character */
			filter->status++;
		} else if (c == 0x5c || c == 0x7e || c < 0 || c > 0x7f) {	/* illegal character */
			filter->flag = 1;	/* bad */
		}
		break;

	/* Modified Base64 */
	case 1:
	case 2:
		n = 0;
		if (c >= 0x41 && c <= 0x5a) {		/* A - Z */
			n = 1;
		} else if (c >= 0x61 && c <= 0x7a) {	/* a - z */
			n = 1;
		} else if (c >= 0x30 && c <= 0x39) {	/* 0 - 9 */
			n = 1;
		} else if (c == 0x2b) {			/* '+' */
			n = 1;
		} else if (c == 0x2f) {			/* '/' */
			n = 1;
		}
		if (n <= 0) {
			if (filter->status == 1 && c != 0x2d) {
				filter->flag = 1;	/* bad */
			} else if (c < 0 || c > 0x7f) {
				filter->flag = 1;	/* bad */
			}
			filter->status = 0;
		} else {
			filter->status = 2;
		}
		break;

	default:
		filter->status = 0;
		break;
	}

	return c;
}

#if defined(HAVE_MBSTR_JA)
static int
mbfl_filt_ident_eucjp(int c, mbfl_identify_filter *filter)
{
	switch (filter->status) {
	case  0:	/* latin */
		if (c >= 0 && c < 0x80) {	/* ok */
			;
		} else if (c > 0xa0 && c < 0xff) {	/* kanji first char */
			filter->status = 1;
		} else if (c == 0x8e) {				/* kana first char */
			filter->status = 2;
		} else if (c == 0x8f) {				/* X 0212 first char */
			filter->status = 3;
		} else {							/* bad */
			filter->flag = 1;
		}
		break;

	case  1:	/* got first half */
		if (c < 0xa1 || c > 0xfe) {		/* bad */
			filter->flag = 1;
		}
		filter->status = 0;
		break;

	case  2:	/* got 0x8e */
		if (c < 0xa1 || c > 0xdf) {		/* bad */
			filter->flag = 1;
		}
		filter->status = 0;
		break;

	case  3:	/* got 0x8f */
		if (c < 0xa1 || c > 0xfe) {		/* bad */
			filter->flag = 1;
		}
		filter->status++;
		break;
	case  4:	/* got 0x8f */
		if (c < 0xa1 || c > 0xfe) {		/* bad */
			filter->flag = 1;
		}
		filter->status = 0;
		break;

	default:
		filter->status = 0;
		break;
	}

	return c;
}

static int
mbfl_filt_ident_sjis(int c, mbfl_identify_filter *filter)
{
	if (filter->status) {		/* kanji second char */
		if (c < 0x40 || c > 0xfc || c == 0x7f) {	/* bad */
		    filter->flag = 1;
		}
		filter->status = 0;
	} else if (c >= 0 && c < 0x80) {	/* latin  ok */
		;
	} else if (c > 0xa0 && c < 0xe0) {	/* kana  ok */
		;
	} else if (c > 0x80 && c < 0xf0 && c != 0xa0) {	/* kanji first char */
		filter->status = 1;
	} else {							/* bad */
		filter->flag = 1;
	}

	return c;
}

static int
mbfl_filt_ident_sjiswin(int c, mbfl_identify_filter *filter)
{
	if (filter->status) {		/* kanji second char */
		if (c < 0x40 || c > 0xfc || c == 0x7f) {	/* bad */
		    filter->flag = 1;
		}
		filter->status = 0;
	} else if (c >= 0 && c < 0x80) {	/* latin  ok */
		;
	} else if (c > 0xa0 && c < 0xe0) {	/* kana  ok */
		;
	} else if (c > 0x80 && c < 0xfd && c != 0xa0) {	/* kanji first char */
		filter->status = 1;
	} else {							/* bad */
		filter->flag = 1;
	}

	return c;
}

static int
mbfl_filt_ident_jis(int c, mbfl_identify_filter *filter)
{
retry:
	switch (filter->status & 0xf) {
/*	case 0x00:	 ASCII */
/*	case 0x10:	 X 0201 latin */
/*	case 0x20:	 X 0201 kana */
/*	case 0x80:	 X 0208 */
/*	case 0x90:	 X 0212 */
	case 0:
		if (c == 0x1b) {
			filter->status += 2;
		} else if (c == 0x0e) {			/* "kana in" */
			filter->status = 0x20;
		} else if (c == 0x0f) {			/* "kana out" */
			filter->status = 0;
		} else if ((filter->status == 0x80 || filter->status == 0x90) && c > 0x20 && c < 0x7f) {		/* kanji first char */
			filter->status += 1;
		} else if (c >= 0 && c < 0x80) {		/* latin, CTLs */
			;
		} else {
			filter->flag = 1;	/* bad */
		}
		break;

/*	case 0x81:	 X 0208 second char */
/*	case 0x91:	 X 0212 second char */
	case 1:
		filter->status &= ~0xf;
		if (c == 0x1b) {
			goto retry;
		} else if (c < 0x21 || c > 0x7e) {		/* bad */
			filter->flag = 1;
		}
		break;

	/* ESC */
	case 2:
		if (c == 0x24) {		/* '$' */
			filter->status++;
		} else if (c == 0x28) {		/* '(' */
			filter->status += 3;
		} else {
			filter->flag = 1;	/* bad */
			filter->status &= ~0xf;
			goto retry;
		}
		break;

	/* ESC $ */
	case 3:
		if (c == 0x40 || c == 0x42) {		/* '@' or 'B' */
			filter->status = 0x80;
		} else if (c == 0x28) {		/* '(' */
			filter->status++;
		} else {
			filter->flag = 1;	/* bad */
			filter->status &= ~0xf;
			goto retry;
		}
		break;

	/* ESC $ ( */
	case 4:
		if (c == 0x40 || c == 0x42) {		/* '@' or 'B' */
			filter->status = 0x80;
		} else if (c == 0x44) {		/* 'D' */
			filter->status = 0x90;
		} else {
			filter->flag = 1;	/* bad */
			filter->status &= ~0xf;
			goto retry;
		}
		break;

	/* ESC ( */
	case 5:
		if (c == 0x42 || c == 0x48) {		/* 'B' or 'H' */
			filter->status = 0;
		} else if (c == 0x4a) {		/* 'J' */
			filter->status = 0x10;
		} else if (c == 0x49) {		/* 'I' */
			filter->status = 0x20;
		} else {
			filter->flag = 1;	/* bad */
			filter->status &= ~0xf;
			goto retry;
		}
		break;

	default:
		filter->status = 0;
		break;
	}

	return c;
}
#endif /* HAVE_MBSTR_JA */

#if defined(HAVE_MBSTR_CN)
static int
mbfl_filt_ident_euccn(int c, mbfl_identify_filter *filter)
{
	switch (filter->status) {
	case  0:	/* latin */
		if (c >= 0 && c < 0x80) {	/* ok */
			;
		} else if (c > 0xa0 && c < 0xff) {	/* DBCS lead byte */
			filter->status = 1;
		} else {							/* bad */
			filter->flag = 1;
		}
		break;

	case  1:	/* got lead byte */
		if (c < 0xa1 || c > 0xfe) {		/* bad */
			filter->flag = 1;
		}
		filter->status = 0;
		break;

	default:
		filter->status = 0;
		break;
	}

	return c;
}

static int
mbfl_filt_ident_cp936(int c, mbfl_identify_filter *filter)
{
	if (filter->status) {		/* kanji second char */
		if (c < 0x40 || c > 0xfe || c == 0x7f) {	/* bad */
		    filter->flag = 1;
		}
		filter->status = 0;
	} else if (c >= 0 && c < 0x80) {	/* latin  ok */
		;
	} else if (c > 0x80 && c < 0xff) {	/* DBCS lead byte */
		filter->status = 1;
	} else {							/* bad */
		filter->flag = 1;
	}

	return c;
}

static int
mbfl_filt_ident_hz(int c, mbfl_identify_filter *filter)
{
	switch (filter->status & 0xf) {
/*	case 0x00:	 ASCII */
/*	case 0x10:	 GB2312 */
	case 0:
		if (c == 0x7e) {
			filter->status += 2;
		} else if (filter->status == 0x10 && c > 0x20 && c < 0x7f) {		/* DBCS first char */
			filter->status += 1;
		} else if (c >= 0 && c < 0x80) {		/* latin, CTLs */
			;
		} else {
			filter->flag = 1;	/* bad */
		}
		break;

/*	case 0x11:	 GB2312 second char */
	case 1:
		filter->status &= ~0xf;
		if (c < 0x21 || c > 0x7e) {		/* bad */
			filter->flag = 1;
		}
		break;

	case 2:
		if (c == 0x7d) {		/* '}' */
			filter->status = 0;
		} else if (c == 0x7b) {		/* '{' */
			filter->status = 0x10;
		} else if (c == 0x7e) {		/* '~' */
			filter->status = 0;
		} else {
			filter->flag = 1;	/* bad */
			filter->status &= ~0xf;
		}
		break;

	default:
		filter->status = 0;
		break;
	}

	return c;
}

#endif /* HAVE_MBSTR_CN */

#if defined(HAVE_MBSTR_TW)
static int
mbfl_filt_ident_euctw(int c, mbfl_identify_filter *filter)
{
	switch (filter->status) {
	case  0:	/* latin */
		if (c >= 0 && c < 0x80) {	/* ok */
			;
		} else if (c > 0xa0 && c < 0xff) {	/* DBCS lead byte */
			filter->status = 1;
		} else if (c == 0x8e) {	/* DBCS lead byte */
			filter->status = 2;
		} else {							/* bad */
			filter->flag = 1;
		}
		break;

	case  1:	/* got lead byte */
		if (c < 0xa1 || c > 0xfe) {		/* bad */
			filter->flag = 1;
		}
		filter->status = 0;
		break;

	case  2:	/* got lead byte */
		if (c >= 0xa1 && c < 0xaf) {	/* ok */
			filter->status = 3;
		} else {
			filter->flag = 1; /* bad */
		}
		break;

	case  3:	/* got lead byte */
		if (c < 0xa1 || c > 0xfe) {		/* bad */
			filter->flag = 1;
		}
		filter->status = 4;
		break;

	case  4:	/* got lead byte */
		if (c < 0xa1 || c > 0xfe) {		/* bad */
			filter->flag = 1;
		}
		filter->status = 0;
		break;

	default:
		filter->status = 0;
		break;
	}

	return c;
}

static int
mbfl_filt_ident_big5(int c, mbfl_identify_filter *filter)
{
	if (filter->status) {		/* kanji second char */
		if (c < 0x40 || (c > 0x7e && c < 0xa1) ||c > 0xfe) {	/* bad */
		    filter->flag = 1;
		}
		filter->status = 0;
	} else if (c >= 0 && c < 0x80) {	/* latin  ok */
		;
	} else if (c > 0xa0 && c < 0xff) {	/* DBCS lead byte */
		filter->status = 1;
	} else {							/* bad */
		filter->flag = 1;
	}

	return c;
}

#endif /* HAVE_MBSTR_TW */

#if defined(HAVE_MBSTR_KR)
static int
mbfl_filt_ident_euckr(int c, mbfl_identify_filter *filter)
{
	switch (filter->status) {
	case  0:	/* latin */
		if (c >= 0 && c < 0x80) {	/* ok */
			;
		} else if (c > 0xa0 && c < 0xff) {	/* DBCS lead byte */
			filter->status = 1;
		} else {							/* bad */
			filter->flag = 1;
		}
		break;

	case  1:	/* got lead byte */
		if (c < 0xa1 || c > 0xfe) {		/* bad */
			filter->flag = 1;
		}
		filter->status = 0;
		break;

	default:
		filter->status = 0;
		break;
	}

	return c;
}

static int
mbfl_filt_ident_uhc(int c, mbfl_identify_filter *filter)
{
	switch (filter->status) {
	case 0: /* latin */
		if (c >= 0 && c < 0x80) { /* ok */
			;
		} else if (c >= 0x81 && c <= 0xa0) {	/* dbcs first char */
		    filter->status= 1;
		} else if (c >= 0xa1 && c <= 0xc6) {	/* dbcs first char */
		    filter->status= 2;
		} else if (c >= 0xc7 && c <= 0xfe) {	/* dbcs first char */
		    filter->status= 3;
		} else { /* bad */
			filter->flag = 1;
		}		

	case 1:
	case 2:
		if (c < 0x41 || (c > 0x5a && c < 0x61)
			|| (c > 0x7a && c < 0x81) || c > 0xfe) {	/* bad */
		    filter->flag = 1;
		}
		filter->status = 0;
		break;

	case 3:
		if (c < 0xa1 || c > 0xfe) {	/* bad */
		    filter->flag = 1;
		}
		filter->status = 0;
		break;

	default:
		filter->status = 0;
		break;
	}

	return c;
}

static int
mbfl_filt_ident_2022kr(int c, mbfl_identify_filter *filter)
{
retry:
	switch (filter->status & 0xf) {
/*	case 0x00:	 ASCII */
/*	case 0x10:	 KSC5601 mode */
/*	case 0x20:	 KSC5601 DBCS */
/*	case 0x40:	 KSC5601 SBCS */
	case 0:
		if (!(filter->status & 0x10)) {
			if (c == 0x1b)
				filter->status += 2;
		} else if (filter->status == 0x20 && c > 0x20 && c < 0x7f) {		/* kanji first char */
			filter->status += 1;
		} else if (c >= 0 && c < 0x80) {		/* latin, CTLs */
			;
		} else {
			filter->flag = 1;	/* bad */
		}
		break;

/*	case 0x21:	 KSC5601 second char */
	case 1:
		filter->status &= ~0xf;
		if (c < 0x21 || c > 0x7e) {		/* bad */
			filter->flag = 1;
		}
		break;

	/* ESC */
	case 2:
		if (c == 0x24) {		/* '$' */
			filter->status++;
		} else {
			filter->flag = 1;	/* bad */
			filter->status &= ~0xf;
			goto retry;
		}
		break;

	/* ESC $ */
	case 3:
		if (c == 0x29) {		/* ')' */
			filter->status++;
		} else {
			filter->flag = 1;	/* bad */
			filter->status &= ~0xf;
			goto retry;
		}
		break;

	/* ESC $) */
	case 5:
		if (c == 0x43) {		/* 'C' */
			filter->status = 0x10;
		} else {
			filter->flag = 1;	/* bad */
			filter->status &= ~0xf;
			goto retry;
		}
		break;

	default:
		filter->status = 0;
		break;
	}

	return c;
}

#endif /* HAVE_MBSTR_KR */


/* We only distinguish the MS extensions to ISO-8859-1.
 * Actually, this is pretty much a NO-OP, since the identification
 * system doesn't allow us to discriminate between a positive match,
 * a possible match and a definite non-match.
 * The problem here is that cp1252 looks like SJIS for certain chars.
 * */
static int
mbfl_filt_ident_cp1252(int c, mbfl_identify_filter *filter)
{
	if (c >= 0x80 && c < 0xa0)
		filter->flag = 0;
	else
		filter->flag = 1; /* not it */
	return c;	
}

#if defined(HAVE_MBSTR_RU)
/* all of this is so ugly now! */
static int
mbfl_filt_ident_cp1251(int c, mbfl_identify_filter *filter)
{
	if (c >= 0x80 && c < 0xff)
		filter->flag = 0;
	else
		filter->flag = 1; /* not it */
	return c;	
}

static int
mbfl_filt_ident_cp866(int c, mbfl_identify_filter *filter)
{
	if (c >= 0x80 && c < 0xff)
		filter->flag = 0;
	else
		filter->flag = 1; /* not it */
	return c;	
}

static int
mbfl_filt_ident_koi8r(int c, mbfl_identify_filter *filter)
{
	if (c >= 0x80 && c < 0xff)
		filter->flag = 0;
	else
		filter->flag = 1; /* not it */
	return c;	
}
#endif /* HAVE_MBSTR_RU */

static int
mbfl_filt_ident_2022jp(int c, mbfl_identify_filter *filter)
{
retry:
	switch (filter->status & 0xf) {
/*	case 0x00:	 ASCII */
/*	case 0x10:	 X 0201 latin */
/*	case 0x80:	 X 0208 */
	case 0:
		if (c == 0x1b) {
			filter->status += 2;
		} else if (filter->status == 0x80 && c > 0x20 && c < 0x7f) {		/* kanji first char */
			filter->status += 1;
		} else if (c >= 0 && c < 0x80) {		/* latin, CTLs */
			;
		} else {
			filter->flag = 1;	/* bad */
		}
		break;

/*	case 0x81:	 X 0208 second char */
	case 1:
		if (c == 0x1b) {
			filter->status++;
		} else {
			filter->status &= ~0xf;
			if (c < 0x21 || c > 0x7e) {		/* bad */
				filter->flag = 1;
			}
		}
		break;

	/* ESC */
	case 2:
		if (c == 0x24) {		/* '$' */
			filter->status++;
		} else if (c == 0x28) {		/* '(' */
			filter->status += 3;
		} else {
			filter->flag = 1;	/* bad */
			filter->status &= ~0xf;
			goto retry;
		}
		break;

	/* ESC $ */
	case 3:
		if (c == 0x40 || c == 0x42) {		/* '@' or 'B' */
			filter->status = 0x80;
		} else {
			filter->flag = 1;	/* bad */
			filter->status &= ~0xf;
			goto retry;
		}
		break;

	/* ESC ( */
	case 5:
		if (c == 0x42) {		/* 'B' */
			filter->status = 0;
		} else if (c == 0x4a) {		/* 'J' */
			filter->status = 0x10;
		} else {
			filter->flag = 1;	/* bad */
			filter->status &= ~0xf;
			goto retry;
		}
		break;

	default:
		filter->status = 0;
		break;
	}

	return c;
}

static void
mbfl_filt_ident_false_ctor(mbfl_identify_filter *filter)
{
	filter->status = 0;
	filter->flag = 1;
}

static int
mbfl_filt_ident_false(int c, mbfl_identify_filter *filter)
{
	filter->flag = 1;	/* bad */
	return c;
}

static int
mbfl_filt_ident_true(int c, mbfl_identify_filter *filter)
{
	return c;
}



/*
 * convert filter
 */

/* setup filter function table */
static void
mbfl_convert_filter_set_vtbl(mbfl_convert_filter *filter, const struct mbfl_convert_vtbl *vtbl)
{
	if (filter && vtbl) {
		filter->filter_ctor = vtbl->filter_ctor;
		filter->filter_dtor = vtbl->filter_dtor;
		filter->filter_function = vtbl->filter_function;
		filter->filter_flush = vtbl->filter_flush;
	}
}


static const struct mbfl_convert_vtbl *
mbfl_convert_filter_get_vtbl(enum mbfl_no_encoding from, enum mbfl_no_encoding to)
{
	const struct mbfl_convert_vtbl *vtbl;
	int i;

	if (to == mbfl_no_encoding_base64 ||
	    to == mbfl_no_encoding_qprint ||
	    to == mbfl_no_encoding_7bit) {
		from = mbfl_no_encoding_8bit;
	} else if (from == mbfl_no_encoding_base64 ||
	           from == mbfl_no_encoding_qprint ||
				  from == mbfl_no_encoding_uuencode) {
		to = mbfl_no_encoding_8bit;
	}

	i = 0;
	while ((vtbl = mbfl_convert_filter_list[i++]) != NULL){
		if (vtbl->from == from && vtbl->to == to) {
			return vtbl;
		}
	}

	return NULL;
}


static void
mbfl_convert_filter_select_vtbl(mbfl_convert_filter *filter)
{
	const struct mbfl_convert_vtbl *vtbl;

	vtbl = mbfl_convert_filter_get_vtbl(filter->from->no_encoding, filter->to->no_encoding);
	if (vtbl == NULL) {
		vtbl = &vtbl_pass;
	}
	mbfl_convert_filter_set_vtbl(filter, vtbl);
}


/* filter pipe */
static int
mbfl_filter_output_pipe(int c, void* data)
{
	mbfl_convert_filter *filter = (mbfl_convert_filter*)data;
	return (*filter->filter_function)(c, filter);
}


/* null output */
static int
mbfl_filter_output_null(int c, void* data)
{
	return c;
}


mbfl_convert_filter *
mbfl_convert_filter_new(
    enum mbfl_no_encoding from,
    enum mbfl_no_encoding to,
    int (*output_function)(int, void* ),
    int (*flush_function)(void*),
    void* data)
{
	mbfl_convert_filter * filter;

	/* allocate */
	filter = (mbfl_convert_filter *)mbfl_malloc(sizeof(mbfl_convert_filter));
	if (filter == NULL) {
		return NULL;
	}

	/* encoding structure */
	filter->from = mbfl_no2encoding(from);
	filter->to = mbfl_no2encoding(to);
	if (filter->from == NULL) {
		filter->from = &mbfl_encoding_pass;
	}
	if (filter->to == NULL) {
		filter->to = &mbfl_encoding_pass;
	}

	if (output_function != NULL) {
		filter->output_function = output_function;
	} else {
		filter->output_function = mbfl_filter_output_null;
	}
	filter->flush_function = flush_function;
	filter->data = data;
	filter->illegal_mode = MBFL_OUTPUTFILTER_ILLEGAL_MODE_CHAR;
	filter->illegal_substchar = 0x3f;		/* '?' */

	/* setup the function table */
	mbfl_convert_filter_select_vtbl(filter);

	/* constructor */
	(*filter->filter_ctor)(filter);

	return filter;
}

void
mbfl_convert_filter_delete(mbfl_convert_filter *filter)
{
	if (filter) {
		(*filter->filter_dtor)(filter);
		mbfl_free((void*)filter);
	}
}

int
mbfl_convert_filter_feed(int c, mbfl_convert_filter *filter)
{
	return (*filter->filter_function)(c, filter);
}

int
mbfl_convert_filter_flush(mbfl_convert_filter *filter)
{
	(*filter->filter_flush)(filter);
	return (filter->flush_function ? (*filter->flush_function)(filter->data) : 0);
}

void
mbfl_convert_filter_reset(
    mbfl_convert_filter *filter,
    enum mbfl_no_encoding from,
    enum mbfl_no_encoding to)
{
	/* destruct old filter */
	(*filter->filter_dtor)(filter);

	/* resset filter member */
	filter->from = mbfl_no2encoding(from);
	filter->to = mbfl_no2encoding(to);

	/* set the vtbl */
	mbfl_convert_filter_select_vtbl(filter);

	/* construct new filter */
	(*filter->filter_ctor)(filter);
}

void
mbfl_convert_filter_copy(
    mbfl_convert_filter *src,
    mbfl_convert_filter *dist)
{
	dist->filter_ctor = src->filter_ctor;
	dist->filter_dtor = src->filter_dtor;
	dist->filter_function = src->filter_function;
	dist->filter_flush = src->filter_flush;
	dist->output_function = src->output_function;
	dist->flush_function = src->flush_function;
	dist->data = src->data;
	dist->status = src->status;
	dist->cache = src->cache;
	dist->from = src->from;
	dist->to = src->to;
	dist->illegal_mode = src->illegal_mode;
	dist->illegal_substchar = src->illegal_substchar;
}

static int
mbfl_convert_filter_devcat(mbfl_convert_filter *filter, mbfl_memory_device *src 
			  )
{
	int n;
	unsigned char *p;

	p = src->buffer;
	n = src->pos;
	while (n > 0) {
		if ((*filter->filter_function)(*p++, filter) < 0) {
			return -1;
		}
		n--;
	}

	return n;
}

static int
mbfl_convert_filter_strcat(mbfl_convert_filter *filter, const unsigned char *p)
{
	int c;

	while ((c = *p++) != '\0') {
		if ((*filter->filter_function)(c, filter) < 0) {
			return -1;
		}
	}

	return 0;
}

#if 0
static int
mbfl_convert_filter_strncat(mbfl_convert_filter *filter, const unsigned char *p, 
			    int n)
{
	while (n > 0) {
		if ((*filter->filter_function)(*p++, filter) < 0) {
			return -1;
		}
		n--;
	}

	return n;
}
#endif

/* illegal character output function for conv-filter */
int
mbfl_filt_conv_illegal_output(int c, mbfl_convert_filter *filter)
{
	int mode_backup, ret, n, m, r;

	ret = 0;
	mode_backup = filter->illegal_mode;
	filter->illegal_mode = MBFL_OUTPUTFILTER_ILLEGAL_MODE_NONE;
	switch (mode_backup) {
	case MBFL_OUTPUTFILTER_ILLEGAL_MODE_CHAR:
		ret = (*filter->filter_function)(filter->illegal_substchar, filter);
		break;
	case MBFL_OUTPUTFILTER_ILLEGAL_MODE_LONG:
		if (c >= 0) {
			if (c < MBFL_WCSGROUP_UCS4MAX) {	/* unicode */
				ret = mbfl_convert_filter_strcat(filter, (const unsigned char *)"U+");
			} else {
				if (c < MBFL_WCSGROUP_WCHARMAX) {
					m = c & ~MBFL_WCSPLANE_MASK;
					switch (m) {
					case MBFL_WCSPLANE_JIS0208:
						ret = mbfl_convert_filter_strcat(filter, (const unsigned char *)"JIS+");
						break;
					case MBFL_WCSPLANE_JIS0212:
						ret = mbfl_convert_filter_strcat(filter, (const unsigned char *)"JIS2+");
						break;
					case MBFL_WCSPLANE_WINCP932:
						ret = mbfl_convert_filter_strcat(filter, (const unsigned char *)"W932+");
						break;
					case MBFL_WCSPLANE_8859_1:
						ret = mbfl_convert_filter_strcat(filter, (const unsigned char *)"I8859_1+");
						break;
					default:
						ret = mbfl_convert_filter_strcat(filter, (const unsigned char *)"?+");
						break;
					}
					c &= MBFL_WCSPLANE_MASK;
				} else {
					ret = mbfl_convert_filter_strcat(filter, (const unsigned char *)"BAD+");
					c &= MBFL_WCSGROUP_MASK;
				}
			}
			if (ret >= 0) {
				m = 0;
				r = 28;
				while (r >= 0) {
					n = (c >> r) & 0xf;
					if (n || m) {
						m = 1;
						ret = (*filter->filter_function)(mbfl_hexchar_table[n], filter);
						if (ret < 0) {
							break;
						}
					}
					r -= 4;
				}
				if (m == 0 && ret >= 0) {
					ret = (*filter->filter_function)(mbfl_hexchar_table[0], filter);
				}
			}
		}
		break;
	default:
		break;
	}
	filter->illegal_mode = mode_backup;

	return ret;
}


/*
 * identify filter
 */

static void
mbfl_identify_filter_set_vtbl(mbfl_identify_filter *filter, const struct mbfl_identify_vtbl *vtbl)
{
	if (filter && vtbl) {
		filter->filter_ctor = vtbl->filter_ctor;
		filter->filter_dtor = vtbl->filter_dtor;
		filter->filter_function = vtbl->filter_function;
	}
}

static const struct mbfl_identify_vtbl * 
mbfl_identify_filter_get_vtbl(enum mbfl_no_encoding encoding)
{
	const struct mbfl_identify_vtbl * vtbl;
	int i;

	i = 0;
	while ((vtbl = mbfl_identify_filter_list[i++]) != NULL) {
		if (vtbl->encoding == encoding) {
			break;
		}
	}

	return vtbl;
}

static void
mbfl_identify_filter_select_vtbl(mbfl_identify_filter *filter)
{
	const struct mbfl_identify_vtbl *vtbl;

	vtbl = mbfl_identify_filter_get_vtbl(filter->encoding->no_encoding);
	if (vtbl == NULL) {
		vtbl = &vtbl_identify_false;
	}
	mbfl_identify_filter_set_vtbl(filter, vtbl);
}

mbfl_identify_filter *
mbfl_identify_filter_new(enum mbfl_no_encoding encoding)
{
	mbfl_identify_filter * filter;

	/* allocate */
	filter = (mbfl_identify_filter *)mbfl_malloc(sizeof(mbfl_identify_filter));
	if (filter == NULL) {
		return NULL;
	}

	/* encoding structure */
	filter->encoding = mbfl_no2encoding(encoding);
	if (filter->encoding == NULL) {
		filter->encoding = &mbfl_encoding_pass;
	}

	filter->status = 0;
	filter->flag = 0;
	filter->score = 0;

	/* setup the function table */
	mbfl_identify_filter_select_vtbl(filter);

	/* constructor */
	(*filter->filter_ctor)(filter);

	return filter;
}

void
mbfl_identify_filter_delete(mbfl_identify_filter *filter)
{
	if (filter) {
		(*filter->filter_dtor)(filter);
		mbfl_free((void*)filter);
	}
}



/*
 *  buffering converter
 */
mbfl_buffer_converter *
mbfl_buffer_converter_new(
    enum mbfl_no_encoding from,
    enum mbfl_no_encoding to,
    int buf_initsz)
{
	mbfl_buffer_converter *convd;

	/* allocate */
	convd = (mbfl_buffer_converter*)mbfl_malloc(sizeof (mbfl_buffer_converter));
	if (convd == NULL) {
		return NULL;
	}

	/* initialize */
	convd->from = mbfl_no2encoding(from);
	convd->to = mbfl_no2encoding(to);
	if (convd->from == NULL) {
		convd->from = &mbfl_encoding_pass;
	}
	if (convd->to == NULL) {
		convd->to = &mbfl_encoding_pass;
	}

	/* create convert filter */
	convd->filter1 = NULL;
	convd->filter2 = NULL;
	if (mbfl_convert_filter_get_vtbl(convd->from->no_encoding, convd->to->no_encoding) != NULL) {
		convd->filter1 = mbfl_convert_filter_new(convd->from->no_encoding, convd->to->no_encoding, mbfl_memory_device_output, 0, &convd->device);
	} else {
		convd->filter2 = mbfl_convert_filter_new(mbfl_no_encoding_wchar, convd->to->no_encoding, mbfl_memory_device_output, 0, &convd->device);
		if (convd->filter2 != NULL) {
			convd->filter1 = mbfl_convert_filter_new(convd->from->no_encoding, mbfl_no_encoding_wchar, (int (*)(int, void*))convd->filter2->filter_function, NULL, convd->filter2);
			if (convd->filter1 == NULL) {
				mbfl_convert_filter_delete(convd->filter2);
			}
		}
	}
	if (convd->filter1 == NULL) {
		return NULL;
	}

	mbfl_memory_device_init(&convd->device, buf_initsz, buf_initsz/4);

	return convd;
}

void
mbfl_buffer_converter_delete(mbfl_buffer_converter *convd)
{
	if (convd != NULL) {
		if (convd->filter1) {
			mbfl_convert_filter_delete(convd->filter1);
		}
		if (convd->filter2) {
			mbfl_convert_filter_delete(convd->filter2);
		}
		mbfl_memory_device_clear(&convd->device);
		mbfl_free((void*)convd);
	}
}

void
mbfl_buffer_converter_reset(mbfl_buffer_converter *convd)
{
	mbfl_memory_device_reset(&convd->device);
}

int
mbfl_buffer_converter_illegal_mode(mbfl_buffer_converter *convd, int mode)
{
	if (convd != NULL) {
		if (convd->filter2 != NULL) {
			convd->filter2->illegal_mode = mode;
		} else if (convd->filter1 != NULL) {
			convd->filter1->illegal_mode = mode;
		} else {
			return 0;
		}
	}

	return 1;
}

int
mbfl_buffer_converter_illegal_substchar(mbfl_buffer_converter *convd, int substchar)
{
	if (convd != NULL) {
		if (convd->filter2 != NULL) {
			convd->filter2->illegal_substchar = substchar;
		} else if (convd->filter1 != NULL) {
			convd->filter1->illegal_substchar = substchar;
		} else {
			return 0;
		}
	}

	return 1;
}

int
mbfl_buffer_converter_strncat(mbfl_buffer_converter *convd, const unsigned char *p, int n)
{
	mbfl_convert_filter *filter;
	int (*filter_function)(int c, mbfl_convert_filter *filter);

	if (convd != NULL && p != NULL) {
		filter = convd->filter1;
		if (filter != NULL) {
			filter_function = filter->filter_function;
			while (n > 0) {
				if ((*filter_function)(*p++, filter) < 0) {
					break;
				}
				n--;
			}
		}
	}

	return n;
}

int
mbfl_buffer_converter_feed(mbfl_buffer_converter *convd, mbfl_string *string)
{
	int n;
	unsigned char *p;
	mbfl_convert_filter *filter;
	int (*filter_function)(int c, mbfl_convert_filter *filter);

	if (convd == NULL || string == NULL) {
		return -1;
	}
	mbfl_memory_device_realloc(&convd->device, convd->device.pos + string->len, string->len/4);
	/* feed data */
	n = string->len;
	p = string->val;
	filter = convd->filter1;
	if (filter != NULL) {
		filter_function = filter->filter_function;
		while (n > 0) {
			if ((*filter_function)(*p++, filter) < 0) {
				return -1;
			}
			n--;
		}
	}

	return 0;
}

int
mbfl_buffer_converter_flush(mbfl_buffer_converter *convd)
{
	if (convd == NULL) {
		return -1;
	}

	if (convd->filter1 != NULL) {
		mbfl_convert_filter_flush(convd->filter1);
	}
	if (convd->filter2 != NULL) {
		mbfl_convert_filter_flush(convd->filter2);
	}

	return 0;
}

mbfl_string *
mbfl_buffer_converter_getbuffer(mbfl_buffer_converter *convd, mbfl_string *result)
{
	if (convd != NULL && result != NULL && convd->device.buffer != NULL) {
		result->no_encoding = convd->to->no_encoding;
		result->val = convd->device.buffer;
		result->len = convd->device.pos;
	} else {
		result = NULL;
	}

	return result;
}

mbfl_string *
mbfl_buffer_converter_result(mbfl_buffer_converter *convd, mbfl_string *result)
{
	if (convd == NULL || result == NULL) {
		return NULL;
	}
	result->no_encoding = convd->to->no_encoding;
	return mbfl_memory_device_result(&convd->device, result);
}

mbfl_string *
mbfl_buffer_converter_feed_result(mbfl_buffer_converter *convd, mbfl_string *string, 
				  mbfl_string *result)
{
	if (convd == NULL || string == NULL || result == NULL) {
		return NULL;
	}
	mbfl_buffer_converter_feed(convd, string);
	if (convd->filter1 != NULL) {
		mbfl_convert_filter_flush(convd->filter1);
	}
	if (convd->filter2 != NULL) {
		mbfl_convert_filter_flush(convd->filter2);
	}
	result->no_encoding = convd->to->no_encoding;
	return mbfl_memory_device_result(&convd->device, result);
}


/*
 * encoding detector
 */
mbfl_encoding_detector *
mbfl_encoding_detector_new(enum mbfl_no_encoding *elist, int eliztsz)
{
	mbfl_encoding_detector *identd;

	int i, num;
	mbfl_identify_filter *filter;

	if (elist == NULL || eliztsz <= 0) {
		return NULL;
	}

	/* allocate */
	identd = (mbfl_encoding_detector*)mbfl_malloc(sizeof(mbfl_encoding_detector));
	if (identd == NULL) {
		return NULL;
	}
	identd->filter_list = (mbfl_identify_filter **)mbfl_calloc(eliztsz, sizeof(mbfl_identify_filter *));
	if (identd->filter_list == NULL) {
		mbfl_free(identd);
		return NULL;
	}

	/* create filters */
	i = 0;
	num = 0;
	while (i < eliztsz) {
		filter = mbfl_identify_filter_new(elist[i]);
		if (filter != NULL) {
			identd->filter_list[num] = filter;
			num++;
		}
		i++;
	}
	identd->filter_list_size = num;

	return identd;
}

void
mbfl_encoding_detector_delete(mbfl_encoding_detector *identd)
{
	int i;

	if (identd != NULL) {
		if (identd->filter_list != NULL) {
			i = identd->filter_list_size;
			while (i > 0) {
				i--;
				mbfl_identify_filter_delete(identd->filter_list[i]);
			}
			mbfl_free((void *)identd->filter_list);
		}
		mbfl_free((void *)identd);
	}
}

int
mbfl_encoding_detector_feed(mbfl_encoding_detector *identd, mbfl_string *string)
{
	int i, n, num, bad, res;
	unsigned char *p;
	mbfl_identify_filter *filter;

	res = 0;
	/* feed data */
	if (identd != NULL && string != NULL && string->val != NULL) {
		num = identd->filter_list_size;
		n = string->len;
		p = string->val;
		while (n > 0) {
			i = 0;
			bad = 0;
			while (i < num) {
				filter = identd->filter_list[i];
				(*filter->filter_function)(*p, filter);
				if (filter->flag) {
					bad++;
				}
				i++;
			}
			if ((num - 1) <= bad) {
				res = 1;
				break;
			}
			p++;
			n--;
		}
	}

	return res;
}

enum mbfl_no_encoding mbfl_encoding_detector_judge(mbfl_encoding_detector *identd)
{
	mbfl_identify_filter *filter;
	enum mbfl_no_encoding encoding;
	int n;

	/* judge */
	encoding = mbfl_no_encoding_invalid;
	if (identd != NULL) {
		n = identd->filter_list_size - 1;
		while (n >= 0) {
			filter = identd->filter_list[n];
			if (!filter->flag) {
				encoding = filter->encoding->no_encoding;
			}
			n--;
		}
	}

	return encoding;
}


/*
 * encoding converter
 */
mbfl_string *
mbfl_convert_encoding(
    mbfl_string *string,
    mbfl_string *result,
    enum mbfl_no_encoding toenc)
{
	int n;
	unsigned char *p;
	const mbfl_encoding *encoding;
	mbfl_memory_device device;
	mbfl_convert_filter *filter1;
	mbfl_convert_filter *filter2;

	/* initialize */
	encoding = mbfl_no2encoding(toenc);
	if (encoding == NULL || string == NULL || result == NULL) {
		return NULL;
	}

	filter1 = NULL;
	filter2 = NULL;
	if (mbfl_convert_filter_get_vtbl(string->no_encoding, toenc) != NULL) {
		filter1 = mbfl_convert_filter_new(string->no_encoding, toenc, mbfl_memory_device_output, 0, &device);
	} else {
		filter2 = mbfl_convert_filter_new(mbfl_no_encoding_wchar, toenc, mbfl_memory_device_output, 0, &device);
		if (filter2 != NULL) {
			filter1 = mbfl_convert_filter_new(string->no_encoding, mbfl_no_encoding_wchar, (int (*)(int, void*))filter2->filter_function, NULL, filter2);
			if (filter1 == NULL) {
				mbfl_convert_filter_delete(filter2);
			}
		}
	}
	if (filter1 == NULL) {
		return NULL;
	}
	filter2->illegal_mode = MBFL_OUTPUTFILTER_ILLEGAL_MODE_CHAR;
	filter2->illegal_substchar = 0x3f;		/* '?' */
	mbfl_memory_device_init(&device, string->len, (string->len >> 2) + 8);

	/* feed data */
	n = string->len;
	p = string->val;
	if (p != NULL) {
		while (n > 0) {
			if ((*filter1->filter_function)(*p++, filter1) < 0) {
				break;
			}
			n--;
		}
	}

	mbfl_convert_filter_flush(filter1);
	mbfl_convert_filter_delete(filter1);
	if (filter2 != NULL) {
		mbfl_convert_filter_flush(filter2);
		mbfl_convert_filter_delete(filter2);
	}

	return mbfl_memory_device_result(&device, result);
}


/*
 * identify encoding
 */
const mbfl_encoding *
mbfl_identify_encoding(mbfl_string *string, enum mbfl_no_encoding *elist, int eliztsz)
{
	int i, n, num, bad;
	unsigned char *p;
	const struct mbfl_identify_vtbl *vtbl;
	mbfl_identify_filter *flist, *filter;
	const mbfl_encoding *encoding;

	/* initialize */
	flist = (mbfl_identify_filter *)mbfl_calloc(eliztsz, sizeof(mbfl_identify_filter));
	if (flist == NULL) {
		return NULL;
	}
	i = 0;
	num = 0;
	if (elist != NULL) {
		while (i < eliztsz) {
			vtbl = mbfl_identify_filter_get_vtbl(elist[i]);
			if (vtbl != NULL) {
				filter = &flist[num];
				mbfl_identify_filter_set_vtbl(filter, vtbl);
				filter->encoding = mbfl_no2encoding(vtbl->encoding);
				(*filter->filter_ctor)(filter);
				num++;
			}
			i++;
		}
	}

	/* feed data */
	n = string->len;
	p = string->val;
	if (p != NULL) {
		while (n > 0) {
			i = 0;
			bad = 0;
			while (i < num) {
				filter = &flist[i];
				(*filter->filter_function)(*p, filter);
				if (filter->flag) {
					bad++;
				}
				i++;
			}
			if ((num - 1) <= bad) {
				break;
			}
			p++;
			n--;
		}
	}

	/* judge */
	i = num - 1;
	bad = 1;
	encoding = NULL;
	while (i >= 0) {
		filter = &flist[i];
		if (filter->flag) {
			bad++;
		} else {
			encoding = filter->encoding;
		}
		i--;
	}
#if 0
	if (bad < num) {
		encoding = NULL;
	}
#endif

	i = 0;
	while (i < num) {
		filter = &flist[i];
		(*filter->filter_dtor)(filter);
		i++;
	}
	mbfl_free((void *)flist);

	return encoding;
}

const char*
mbfl_identify_encoding_name(mbfl_string *string, enum mbfl_no_encoding *elist, int eliztsz)
{
	const mbfl_encoding *encoding;

	encoding = mbfl_identify_encoding(string, elist, eliztsz);
	if (encoding != NULL &&
	    encoding->no_encoding > mbfl_no_encoding_charset_min &&
	    encoding->no_encoding < mbfl_no_encoding_charset_max) {
		return encoding->name;
	} else {
		return NULL;
	}
}

const enum mbfl_no_encoding
mbfl_identify_encoding_no(mbfl_string *string, enum mbfl_no_encoding *elist, int eliztsz)
{
	const mbfl_encoding *encoding;

	encoding = mbfl_identify_encoding(string, elist, eliztsz);
	if (encoding != NULL &&
	    encoding->no_encoding > mbfl_no_encoding_charset_min &&
	    encoding->no_encoding < mbfl_no_encoding_charset_max) {
		return encoding->no_encoding;
	} else {
		return mbfl_no_encoding_invalid;
	}
}


/*
 *  strlen
 */
static int
filter_count_output(int c, void *data)
{
	(*(int *)data)++;
	return c;
}

int
mbfl_strlen(mbfl_string *string)
{
	int len, n, m, k;
	unsigned char *p;
	const unsigned char *mbtab;
	const mbfl_encoding *encoding;

	encoding = mbfl_no2encoding(string->no_encoding);
	if (encoding == NULL || string == NULL) {
		return -1;
	}

	len = 0;
	if (encoding->flag & MBFL_ENCTYPE_SBCS) {
		len = string->len;
	} else if (encoding->flag & (MBFL_ENCTYPE_WCS2BE | MBFL_ENCTYPE_WCS2LE)) {
		len = string->len/2;
	} else if (encoding->flag & (MBFL_ENCTYPE_WCS4BE | MBFL_ENCTYPE_WCS4LE)) {
		len = string->len/4;
	} else if (encoding->mblen_table != NULL) {
		mbtab = encoding->mblen_table;
		n = 0;
		p = string->val;
		k = string->len;
		/* count */
		if (p != NULL) {
			while (n < k) {
				m = mbtab[*p];
				n += m;
				p += m;
				len++;
			};
		}
	} else {
		/* wchar filter */
		mbfl_convert_filter *filter = mbfl_convert_filter_new(
		  string->no_encoding, 
		  mbfl_no_encoding_wchar,
		  filter_count_output, 0, &len);
		if (filter == NULL) {
			return -1;
		}
		/* count */
		n = string->len;
		p = string->val;
		if (p != NULL) {
			while (n > 0) {
				(*filter->filter_function)(*p++, filter);
				n--;
			}
		}
		mbfl_convert_filter_delete(filter);
	}

	return len;
}

#ifdef ZEND_MULTIBYTE
/*
 *	oddlen
 */
int
mbfl_oddlen(mbfl_string *string)
{
	int len, n, m, k;
	unsigned char *p;
	const unsigned char *mbtab;
	const mbfl_encoding *encoding;

	encoding = mbfl_no2encoding(string->no_encoding);
	if (encoding == NULL || string == NULL) {
		return -1;
	}

	len = 0;
	if (encoding->flag & MBFL_ENCTYPE_SBCS) {
		return 0;
	} else if (encoding->flag & (MBFL_ENCTYPE_WCS2BE | MBFL_ENCTYPE_WCS2LE)) {
		return len % 2;
	} else if (encoding->flag & (MBFL_ENCTYPE_WCS4BE | MBFL_ENCTYPE_WCS4LE)) {
		return len % 4;
	} else if (encoding->mblen_table != NULL) {
 		mbtab = encoding->mblen_table;
 		n = 0;
		p = string->val;
		k = string->len;
		/* count */
		if (p != NULL) {
			while (n < k) {
				m = mbtab[*p];
				n += m;
				p += m;
			};
		}
		return n-k;
	} else {
		/* how can i do ? */
		return 0;
	}
	/* NOT REACHED */
}
#endif /* ZEND_MULTIBYTE */
 
 
/*
 *  strpos
 */
struct collector_strpos_data {
	mbfl_convert_filter *next_filter;
	mbfl_wchar_device needle;
	int needle_len;
	int start;
	int output;
	int found_pos;
	int needle_pos;
	int matched_pos;
};

static int
collector_strpos(int c, void* data)
{
	int *p, *h, *m, n;
	struct collector_strpos_data *pc = (struct collector_strpos_data*)data;

	if (pc->output >= pc->start) {
		if (c == (int)pc->needle.buffer[pc->needle_pos]) {
			if (pc->needle_pos == 0) {
				pc->found_pos = pc->output;			/* found position */
			}
			pc->needle_pos++;						/* needle pointer */
			if (pc->needle_pos >= pc->needle_len) {
				pc->matched_pos = pc->found_pos;	/* matched position */
				pc->needle_pos--;
				goto retry;
			}
		} else if (pc->needle_pos != 0) {
retry:
			h = (int *)pc->needle.buffer;
			h++;
			for (;;) {
				pc->found_pos++;
				p = h;
				m = pc->needle.buffer;
				n = pc->needle_pos - 1;
				while (n > 0 && *p == *m) {
					n--;
					p++;
					m++;
				}
				if (n <= 0) {
					if (*m != c) {
						pc->needle_pos = 0;
					}
					break;
				} else {
					h++;
					pc->needle_pos--;
				}
			}
		}
	}

	pc->output++;
	return c;
}

int
mbfl_strpos(
    mbfl_string *haystack,
    mbfl_string *needle,
    int offset,
    int reverse)
{
	int n, result;
	unsigned char *p;
	mbfl_convert_filter *filter;
	struct collector_strpos_data pc;

	if (haystack == NULL || needle == NULL) {
		return -8;
	}
	/* needle is converted into wchar */
	mbfl_wchar_device_init(&pc.needle);
	filter = mbfl_convert_filter_new(
	  needle->no_encoding,
	  mbfl_no_encoding_wchar,
	  mbfl_wchar_device_output, 0, &pc.needle);
	if (filter == NULL) {
		return -4;
	}
	p = needle->val;
	n = needle->len;
	if (p != NULL) {
		while (n > 0) {
			if ((*filter->filter_function)(*p++, filter) < 0) {
				break;
			}
			n--;
		}
	}
	mbfl_convert_filter_flush(filter);
	mbfl_convert_filter_delete(filter);
	pc.needle_len = pc.needle.pos;
	if (pc.needle.buffer == NULL) {
		return -4;
	}
	if (pc.needle_len <= 0) {
		mbfl_wchar_device_clear(&pc.needle);
		return -2;
	}
	/* initialize filter and collector data */
	filter = mbfl_convert_filter_new(
	  haystack->no_encoding,
	  mbfl_no_encoding_wchar,
	  collector_strpos, 0, &pc);
	if (filter == NULL) {
		mbfl_wchar_device_clear(&pc.needle);
		return -4;
	}
	pc.start = offset;
	pc.output = 0;
	pc.needle_pos = 0;
	pc.found_pos = 0;
	pc.matched_pos = -1;

	/* feed data */
	p = haystack->val;
	n = haystack->len;
	if (p != NULL) {
		while (n > 0) {
			if ((*filter->filter_function)(*p++, filter) < 0) {
				pc.matched_pos = -4;
				break;
			}
			if (pc.matched_pos >= 0 && !reverse) {
				break;
			}
			n--;
		}
	}
	mbfl_convert_filter_flush(filter);
	result = pc.matched_pos;
	mbfl_convert_filter_delete(filter);
	mbfl_wchar_device_clear(&pc.needle);

	return result;
}

/*
 *  substr_count
 */

int
mbfl_substr_count(
    mbfl_string *haystack,
    mbfl_string *needle
   )
{
	int n, result = 0;
	unsigned char *p;
	mbfl_convert_filter *filter;
	struct collector_strpos_data pc;

	if (haystack == NULL || needle == NULL) {
		return -8;
	}
	/* needle is converted into wchar */
	mbfl_wchar_device_init(&pc.needle);
	filter = mbfl_convert_filter_new(
	  needle->no_encoding,
	  mbfl_no_encoding_wchar,
	  mbfl_wchar_device_output, 0, &pc.needle);
	if (filter == NULL) {
		return -4;
	}
	p = needle->val;
	n = needle->len;
	if (p != NULL) {
		while (n > 0) {
			if ((*filter->filter_function)(*p++, filter) < 0) {
				break;
			}
			n--;
		}
	}
	mbfl_convert_filter_flush(filter);
	mbfl_convert_filter_delete(filter);
	pc.needle_len = pc.needle.pos;
	if (pc.needle.buffer == NULL) {
		return -4;
	}
	if (pc.needle_len <= 0) {
		mbfl_wchar_device_clear(&pc.needle);
		return -2;
	}
	/* initialize filter and collector data */
	filter = mbfl_convert_filter_new(
	  haystack->no_encoding,
	  mbfl_no_encoding_wchar,
	  collector_strpos, 0, &pc);
	if (filter == NULL) {
		mbfl_wchar_device_clear(&pc.needle);
		return -4;
	}
	pc.start = 0;
	pc.output = 0;
	pc.needle_pos = 0;
	pc.found_pos = 0;
	pc.matched_pos = -1;

	/* feed data */
	p = haystack->val;
	n = haystack->len;
	if (p != NULL) {
		while (n > 0) {
			if ((*filter->filter_function)(*p++, filter) < 0) {
				pc.matched_pos = -4;
				break;
			}
			if (pc.matched_pos >= 0) {
				++result;
				pc.matched_pos = -1;
			}
			n--;
		}
	}
	mbfl_convert_filter_flush(filter);
	mbfl_convert_filter_delete(filter);
	mbfl_wchar_device_clear(&pc.needle);

	return result;
}

/*
 *  substr
 */
struct collector_substr_data {
	mbfl_convert_filter *next_filter;
	int start;
	int stop;
	int output;
};

static int
collector_substr(int c, void* data)
{
	struct collector_substr_data *pc = (struct collector_substr_data*)data;

	if (pc->output >= pc->stop) {
		return -1;
	}

	if (pc->output >= pc->start) {
		(*pc->next_filter->filter_function)(c, pc->next_filter);
	}

	pc->output++;

	return c;
}

mbfl_string *
mbfl_substr(
    mbfl_string *string,
    mbfl_string *result,
    int from,
    int length)
{
	const mbfl_encoding *encoding;
	int n, m, k, len, start, end;
	unsigned char *p, *w;
	const unsigned char *mbtab;

	encoding = mbfl_no2encoding(string->no_encoding);
	if (encoding == NULL || string == NULL || result == NULL) {
		return NULL;
	}
	mbfl_string_init(result);
	result->no_language = string->no_language;
	result->no_encoding = string->no_encoding;

	if ((encoding->flag & (MBFL_ENCTYPE_SBCS | MBFL_ENCTYPE_WCS2BE | MBFL_ENCTYPE_WCS2LE | MBFL_ENCTYPE_WCS4BE | MBFL_ENCTYPE_WCS4LE)) ||
	   encoding->mblen_table != NULL) {
		len = string->len;
		start = from;
		end = from + length;
		if (encoding->flag & (MBFL_ENCTYPE_WCS2BE | MBFL_ENCTYPE_MWC2LE)) {
			start *= 2;
			end = start + length*2;
		} else if (encoding->flag & (MBFL_ENCTYPE_WCS4BE | MBFL_ENCTYPE_MWC4LE)) {
			start *= 4;
			end = start + length*4;
		} else if (encoding->mblen_table != NULL) {
			mbtab = encoding->mblen_table;
			start = 0;
			end = 0;
			n = 0;
			k = 0;
			p = string->val;
			if (p != NULL) {
				/* search start position */
				while (k <= from) {
					start = n;
					if (n >= len) {
						break;
					}
					m = mbtab[*p];
					n += m;
					p += m;
					k++;
				}
				/* detect end position */
				k = 0;
				end = start;
				while (k < length) {
					end = n;
					if (n >= len) {
						break;
					}
					m = mbtab[*p];
					n += m;
					p += m;
					k++;
				}
			}
		}

		if (start > len) {
			start = len;
		}
		if (start < 0) {
			start = 0;
		}
		if (end > len) {
			end = len;
		}
		if (end < 0) {
			end = 0;
		}
		if (start > end) {
			start = end;
		}

		/* allocate memory and copy */
		n = end - start;
		result->len = 0;
		result->val = w = (unsigned char*)mbfl_malloc((n + 8)*sizeof(unsigned char));
		if (w != NULL) {
			p = string->val;
			if (p != NULL) {
				p += start;
				result->len = n;
				while (n > 0) {
					*w++ = *p++;
					n--;
				}
			}
			*w++ = '\0';
			*w++ = '\0';
			*w++ = '\0';
			*w = '\0';
		} else {
			result = NULL;
		}
	} else {
		mbfl_memory_device device;
		struct collector_substr_data pc;
		mbfl_convert_filter *decoder;
		mbfl_convert_filter *encoder;

		mbfl_memory_device_init(&device, length + 1, 0);
		mbfl_string_init(result);
		result->no_language = string->no_language;
		result->no_encoding = string->no_encoding;
		/* output code filter */
		decoder = mbfl_convert_filter_new(
		    mbfl_no_encoding_wchar,
		    string->no_encoding,
		    mbfl_memory_device_output, 0, &device);
		/* wchar filter */
		encoder = mbfl_convert_filter_new(
		    string->no_encoding,
		    mbfl_no_encoding_wchar,
		    collector_substr, 0, &pc);
		if (decoder == NULL || encoder == NULL) {
			mbfl_convert_filter_delete(encoder);
			mbfl_convert_filter_delete(decoder);
			return NULL;
		}
		pc.next_filter = decoder;
		pc.start = from;
		pc.stop = from + length;
		pc.output = 0;

		/* feed data */
		p = string->val;
		n = string->len;
		if (p != NULL) {
			while (n > 0) {
				if ((*encoder->filter_function)(*p++, encoder) < 0) {
					break;
				}
				n--;
			}
		}

		mbfl_convert_filter_flush(encoder);
		mbfl_convert_filter_flush(decoder);
		result = mbfl_memory_device_result(&device, result);
		mbfl_convert_filter_delete(encoder);
		mbfl_convert_filter_delete(decoder);
	}

	return result;
}


/*
 *  strcut
 */
mbfl_string *
mbfl_strcut(
    mbfl_string *string,
    mbfl_string *result,
    int from,
    int length)
{
	const mbfl_encoding *encoding;
	int n, m, k, len, start, end;
	unsigned char *p, *w;
	const unsigned char *mbtab;
	mbfl_memory_device device;
	mbfl_convert_filter *encoder, *encoder_tmp, *decoder, *decoder_tmp;

	encoding = mbfl_no2encoding(string->no_encoding);
	if (encoding == NULL || string == NULL || result == NULL) {
		return NULL;
	}
	mbfl_string_init(result);
	result->no_language = string->no_language;
	result->no_encoding = string->no_encoding;

	if ((encoding->flag & (MBFL_ENCTYPE_SBCS | MBFL_ENCTYPE_WCS2BE | MBFL_ENCTYPE_WCS2LE | MBFL_ENCTYPE_WCS4BE | MBFL_ENCTYPE_WCS4LE)) ||
	   encoding->mblen_table != NULL) {
		len = string->len;
		start = from;
		end = from + length;
		if (encoding->flag & (MBFL_ENCTYPE_WCS2BE | MBFL_ENCTYPE_WCS2LE)) {
			start /= 2;
			start *= 2;
			end = length/2;
			end *= 2;
			end += start;
		} else if (encoding->flag & (MBFL_ENCTYPE_WCS4BE | MBFL_ENCTYPE_WCS4LE)) {
			start /= 4;
			start *= 4;
			end = length/4;
			end *= 4;
			end += start;
		} else if (encoding->mblen_table != NULL) {
			mbtab = encoding->mblen_table;
			start = 0;
			end = 0;
			n = 0;
			p = string->val;
			if (p != NULL) {
				/* search start position */
				for (;;) {
					m = mbtab[*p];
					n += m;
					p += m;
					if (n > from) {
						break;
					}
					start = n;
				}
				/* search end position */
				k = start + length;
				if (k >= (int)string->len) {
					end = string->len;
				} else {
					end = start;
					while (n <= k) {
						end = n;
						m = mbtab[*p];
						n += m;
						p += m;
					}
				}
			}
		}

		if (start > len) {
			start = len;
		}
		if (start < 0) {
			start = 0;
		}
		if (end > len) {
			end = len;
		}
		if (end < 0) {
			end = 0;
		}
		if (start > end) {
			start = end;
		}
		/* allocate memory and copy string */
		n = end - start;
		result->len = 0;
		result->val = w = (unsigned char*)mbfl_malloc((n + 8)*sizeof(unsigned char));
		if (w != NULL) {
			result->len = n;
			p = &(string->val[start]);
			while (n > 0) {
				*w++ = *p++;
				n--;
			}
			*w++ = '\0';
			*w++ = '\0';
			*w++ = '\0';
			*w = '\0';
		} else {
			result = NULL;
		}
	} else {
		/* wchar filter */
		encoder = mbfl_convert_filter_new(
		  string->no_encoding,
		  mbfl_no_encoding_wchar,
		  mbfl_filter_output_null, 0, 0);
		encoder_tmp = mbfl_convert_filter_new(
		  string->no_encoding,
		  mbfl_no_encoding_wchar,
		  mbfl_filter_output_null, 0, 0);
		/* output code filter */
		decoder = mbfl_convert_filter_new(
		  mbfl_no_encoding_wchar,
		  string->no_encoding,
		  mbfl_memory_device_output, 0, &device);
		decoder_tmp = mbfl_convert_filter_new(
		  mbfl_no_encoding_wchar,
		  string->no_encoding,
		  mbfl_memory_device_output, 0, &device);
		if (encoder == NULL || encoder_tmp == NULL || decoder == NULL || decoder_tmp == NULL) {
			mbfl_convert_filter_delete(encoder);
			mbfl_convert_filter_delete(encoder_tmp);
			mbfl_convert_filter_delete(decoder);
			mbfl_convert_filter_delete(decoder_tmp);
			return NULL;
		}
		mbfl_memory_device_init(&device, length + 8, 0);
		k = 0;
		n = 0;
		p = string->val;
		if (p != NULL) {
			/* seartch start position */
			while (n < from) {
				(*encoder->filter_function)(*p++, encoder);
				n++;
			}
			/* output a little shorter than "length" */
			encoder->output_function = mbfl_filter_output_pipe;
			encoder->data = decoder;
			k = length - 20;
			len = string->len;
			while (n < len && device.pos < k) {
				(*encoder->filter_function)(*p++, encoder);
				n++;
			}
			/* detect end position */
			for (;;) {
				/* backup current state */
				k = device.pos;
				mbfl_convert_filter_copy(encoder, encoder_tmp);
				mbfl_convert_filter_copy(decoder, decoder_tmp);
				if (n >= len) {
					break;
				}
				/* feed 1byte and flush */
				(*encoder->filter_function)(*p, encoder);
				(*encoder->filter_flush)(encoder);
				(*decoder->filter_flush)(decoder);
				if (device.pos > length) {
					break;
				}
				/* restore filter and re-feed data */
				device.pos = k;
				mbfl_convert_filter_copy(encoder_tmp, encoder);
				mbfl_convert_filter_copy(decoder_tmp, decoder);
				(*encoder->filter_function)(*p, encoder);
				p++;
				n++;
			}
			device.pos = k;
			mbfl_convert_filter_copy(encoder_tmp, encoder);
			mbfl_convert_filter_copy(decoder_tmp, decoder);
			mbfl_convert_filter_flush(encoder);
			mbfl_convert_filter_flush(decoder);
		}
		result = mbfl_memory_device_result(&device, result);
		mbfl_convert_filter_delete(encoder);
		mbfl_convert_filter_delete(encoder_tmp);
		mbfl_convert_filter_delete(decoder);
		mbfl_convert_filter_delete(decoder_tmp);
	}

	return result;
}


/*
 *  strwidth
 */
static int
filter_count_width(int c, void* data)
{
	if (c >= 0x20) {
		if (c < 0x2000 || (c > 0xff60 && c < 0xffa0)) {
			(*(int *)data)++;
		} else {
			(*(int *)data) += 2;
		}
	}

	return c;
}

int
mbfl_strwidth(mbfl_string *string)
{
	int len, n;
	unsigned char *p;
	mbfl_convert_filter *filter;

	len = 0;
	if (string->len > 0 && string->val != NULL) {
		/* wchar filter */
		filter = mbfl_convert_filter_new(
		    string->no_encoding,
		    mbfl_no_encoding_wchar,
		    filter_count_width, 0, &len);
		if (filter == NULL) {
			mbfl_convert_filter_delete(filter);
			return -1;
		}

		/* feed data */
		p = string->val;
		n = string->len;
		while (n > 0) {
			(*filter->filter_function)(*p++, filter);
			n--;
		}

		mbfl_convert_filter_flush(filter);
		mbfl_convert_filter_delete(filter);
	}

	return len;
}


/*
 *  strimwidth
 */
struct collector_strimwidth_data {
	mbfl_convert_filter *decoder;
	mbfl_convert_filter *decoder_backup;
	mbfl_memory_device device;
	int from;
	int width;
	int outwidth;
	int outchar;
	int status;
	int endpos;
};

static int
collector_strimwidth(int c, void* data)
{
	struct collector_strimwidth_data *pc = (struct collector_strimwidth_data*)data;

	switch (pc->status) {
	case 10:
		(*pc->decoder->filter_function)(c, pc->decoder);
		break;
	default:
		if (pc->outchar >= pc->from) {
			if (c >= 0x20) {
				if (c < 0x2000 || (c > 0xff60 && c < 0xffa0)) {
					pc->outwidth++;
				} else {
					pc->outwidth += 2;
				}
			}
			if (pc->outwidth > pc->width) {
				if (pc->status == 0) {
					pc->endpos = pc->device.pos;
					mbfl_convert_filter_copy(pc->decoder, pc->decoder_backup);
				}
				pc->status++;
				(*pc->decoder->filter_function)(c, pc->decoder);
				c = -1;
			} else {
				(*pc->decoder->filter_function)(c, pc->decoder);
			}
		}
		pc->outchar++;
		break;
	}

	return c;
}

mbfl_string *
mbfl_strimwidth(
    mbfl_string *string,
    mbfl_string *marker,
    mbfl_string *result,
    int from,
    int width)
{
	struct collector_strimwidth_data pc;
	mbfl_convert_filter *encoder;
	int n, mkwidth;
	unsigned char *p;

	if (string == NULL || result == NULL) {
		return NULL;
	}
	mbfl_string_init(result);
	result->no_language = string->no_language;
	result->no_encoding = string->no_encoding;
	mbfl_memory_device_init(&pc.device, width, 0);

	/* output code filter */
	pc.decoder = mbfl_convert_filter_new(
	    mbfl_no_encoding_wchar,
	    string->no_encoding,
	    mbfl_memory_device_output, 0, &pc.device);
	pc.decoder_backup = mbfl_convert_filter_new(
	    mbfl_no_encoding_wchar,
	    string->no_encoding,
	    mbfl_memory_device_output, 0, &pc.device);
	/* wchar filter */
	encoder = mbfl_convert_filter_new(
	    string->no_encoding,
	    mbfl_no_encoding_wchar,
	    collector_strimwidth, 0, &pc);
	if (pc.decoder == NULL || pc.decoder_backup == NULL || encoder == NULL) {
		mbfl_convert_filter_delete(encoder);
		mbfl_convert_filter_delete(pc.decoder);
		mbfl_convert_filter_delete(pc.decoder_backup);
		return NULL;
	}
	mkwidth = 0;
	if (marker) {
		mkwidth = mbfl_strwidth(marker);
	}
	pc.from = from;
	pc.width = width - mkwidth;
	pc.outwidth = 0;
	pc.outchar = 0;
	pc.status = 0;
	pc.endpos = 0;

	/* feed data */
	p = string->val;
	n = string->len;
	if (p != NULL) {
		while (n > 0) {
			n--;
			if ((*encoder->filter_function)(*p++, encoder) < 0) {
				break;
			}
		}
		mbfl_convert_filter_flush(encoder);
		if (pc.status != 0 && mkwidth > 0) {
			pc.width += mkwidth;
			while (n > 0) {
				if ((*encoder->filter_function)(*p++, encoder) < 0) {
					break;
				}
				n--;
			}
			mbfl_convert_filter_flush(encoder);
			if (pc.status != 1) {
				pc.status = 10;
				pc.device.pos = pc.endpos;
				mbfl_convert_filter_copy(pc.decoder_backup, pc.decoder);
				mbfl_convert_filter_reset(encoder, marker->no_encoding, mbfl_no_encoding_wchar);
				p = marker->val;
				n = marker->len;
				while (n > 0) {
					if ((*encoder->filter_function)(*p++, encoder) < 0) {
						break;
					}
					n--;
				}
				mbfl_convert_filter_flush(encoder);
			}
		} else if (pc.status != 0) {
			pc.device.pos = pc.endpos;
			mbfl_convert_filter_copy(pc.decoder_backup, pc.decoder);
		}
		mbfl_convert_filter_flush(pc.decoder);
	}
	result = mbfl_memory_device_result(&pc.device, result);
	mbfl_convert_filter_delete(encoder);
	mbfl_convert_filter_delete(pc.decoder);
	mbfl_convert_filter_delete(pc.decoder_backup);

	return result;
}



/*
 *  convert Hankaku and Zenkaku
 */
struct collector_hantozen_data {
	mbfl_convert_filter *next_filter;
	int mode;
	int status;
	int cache;
};

static const unsigned char hankana2zenkata_table[64] = {
	0x00,0x02,0x0C,0x0D,0x01,0xFB,0xF2,0xA1,0xA3,0xA5,
	0xA7,0xA9,0xE3,0xE5,0xE7,0xC3,0xFC,0xA2,0xA4,0xA6,
	0xA8,0xAA,0xAB,0xAD,0xAF,0xB1,0xB3,0xB5,0xB7,0xB9,
	0xBB,0xBD,0xBF,0xC1,0xC4,0xC6,0xC8,0xCA,0xCB,0xCC,
	0xCD,0xCE,0xCF,0xD2,0xD5,0xD8,0xDB,0xDE,0xDF,0xE0,
	0xE1,0xE2,0xE4,0xE6,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,
	0xEF,0xF3,0x9B,0x9C
};
static const unsigned char hankana2zenhira_table[64] = {
	0x00,0x02,0x0C,0x0D,0x01,0xFB,0x92,0x41,0x43,0x45,
	0x47,0x49,0x83,0x85,0x87,0x63,0xFC,0x42,0x44,0x46,
	0x48,0x4A,0x4B,0x4D,0x4F,0x51,0x53,0x55,0x57,0x59,
	0x5B,0x5D,0x5F,0x61,0x64,0x66,0x68,0x6A,0x6B,0x6C,
	0x6D,0x6E,0x6F,0x72,0x75,0x78,0x7B,0x7E,0x7F,0x80,
	0x81,0x82,0x84,0x86,0x88,0x89,0x8A,0x8B,0x8C,0x8D,
	0x8F,0x93,0x9B,0x9C
};
static const unsigned char zenkana2hankana_table[84][2] = {
	{0x67,0x00},{0x71,0x00},{0x68,0x00},{0x72,0x00},{0x69,0x00},
	{0x73,0x00},{0x6A,0x00},{0x74,0x00},{0x6B,0x00},{0x75,0x00},
	{0x76,0x00},{0x76,0x9E},{0x77,0x00},{0x77,0x9E},{0x78,0x00},
	{0x78,0x9E},{0x79,0x00},{0x79,0x9E},{0x7A,0x00},{0x7A,0x9E},
	{0x7B,0x00},{0x7B,0x9E},{0x7C,0x00},{0x7C,0x9E},{0x7D,0x00},
	{0x7D,0x9E},{0x7E,0x00},{0x7E,0x9E},{0x7F,0x00},{0x7F,0x9E},
	{0x80,0x00},{0x80,0x9E},{0x81,0x00},{0x81,0x9E},{0x6F,0x00},
	{0x82,0x00},{0x82,0x9E},{0x83,0x00},{0x83,0x9E},{0x84,0x00},
	{0x84,0x9E},{0x85,0x00},{0x86,0x00},{0x87,0x00},{0x88,0x00},
	{0x89,0x00},{0x8A,0x00},{0x8A,0x9E},{0x8A,0x9F},{0x8B,0x00},
	{0x8B,0x9E},{0x8B,0x9F},{0x8C,0x00},{0x8C,0x9E},{0x8C,0x9F},
	{0x8D,0x00},{0x8D,0x9E},{0x8D,0x9F},{0x8E,0x00},{0x8E,0x9E},
	{0x8E,0x9F},{0x8F,0x00},{0x90,0x00},{0x91,0x00},{0x92,0x00},
	{0x93,0x00},{0x6C,0x00},{0x94,0x00},{0x6D,0x00},{0x95,0x00},
	{0x6E,0x00},{0x96,0x00},{0x97,0x00},{0x98,0x00},{0x99,0x00},
	{0x9A,0x00},{0x9B,0x00},{0x9C,0x00},{0x9C,0x00},{0x72,0x00},
	{0x74,0x00},{0x66,0x00},{0x9D,0x00},{0x73,0x9E}
};

static int
collector_hantozen(int c, void* data)
{
	int s, mode, n;
	struct collector_hantozen_data *pc = (struct collector_hantozen_data*)data;

	s = c;
	mode = pc->mode;

	if (mode & 0xf) { /* hankaku to zenkaku */
		if ((mode & 0x1) && c >= 0x21 && c <= 0x7d && c != 0x22 && c != 0x27 && c != 0x5c) {	/* all except <"> <'> <\> <~> */
			s = c + 0xfee0;
		} else if ((mode & 0x2) && ((c >= 0x41 && c <= 0x5a) || (c >= 0x61 && c <= 0x7a))) {	/* alpha */
			s = c + 0xfee0;
		} else if ((mode & 0x4) && c >= 0x30 && c <= 0x39) {	/* num */
			s = c + 0xfee0;
		} else if ((mode & 0x8) && c == 0x20) {	/* spase */
			s = 0x3000;
		}
	}

	if (mode & 0xf0) { /* zenkaku to hankaku */
		if ((mode & 0x10) && c >= 0xff01 && c <= 0xff5d && c != 0xff02 && c != 0xff07 && c!= 0xff3c) {	/* all except <"> <'> <\> <~> */
			s = c - 0xfee0;
		} else if ((mode & 0x20) && ((c >= 0xff21 && c <= 0xff3a) || (c >= 0xff41 && c <= 0xff5a))) {	/* alpha */
			s = c - 0xfee0;
		} else if ((mode & 0x40) && (c >= 0xff10 && c <= 0xff19)) {	/* num */
			s = c - 0xfee0;
		} else if ((mode & 0x80) && (c == 0x3000)) {	/* spase */
			s = 0x20;
		} else if ((mode & 0x10) && (c == 0x2212)) {	/* MINUS SIGN */
			s = 0x2d;
		}
	}

	if (mode & 0x300) {	/* hankaku kana to zenkaku kana */
		if ((mode & 0x100) && (mode & 0x800)) {	/* hankaku kana to zenkaku katakana and glue voiced sound mark */
			if (c >= 0xff61 && c <= 0xff9f) {
				if (pc->status) {
					n = (pc->cache - 0xff60) & 0x3f;
					if (c == 0xff9e && ((n >= 22 && n <= 36) || (n >= 42 && n <= 46))) {
						pc->status = 0;
						s = 0x3001 + hankana2zenkata_table[n];
					} else if (c == 0xff9e && n == 19) {
						pc->status = 0;
						s = 0x30f4;
					} else if (c == 0xff9f && (n >= 42 && n <= 46)) {
						pc->status = 0;
						s = 0x3002 + hankana2zenkata_table[n];
					} else {
						pc->status = 1;
						pc->cache = c;
						s = 0x3000 + hankana2zenkata_table[n];
					}
				} else {
					pc->status = 1;
					pc->cache = c;
					return c;
				}
			} else {
				if (pc->status) {
					n = (pc->cache - 0xff60) & 0x3f;
					pc->status = 0;
					(*pc->next_filter->filter_function)(0x3000 + hankana2zenkata_table[n], pc->next_filter);
				}
			}
		} else if ((mode & 0x200) && (mode & 0x800)) {	/* hankaku kana to zenkaku hirangana and glue voiced sound mark */
			if (c >= 0xff61 && c <= 0xff9f) {
				if (pc->status) {
					n = (pc->cache - 0xff60) & 0x3f;
					if (c == 0xff9e && ((n >= 22 && n <= 36) || (n >= 42 && n <= 46))) {
						pc->status = 0;
						s = 0x3001 + hankana2zenhira_table[n];
					} else if (c == 0xff9f && (n >= 42 && n <= 46)) {
						pc->status = 0;
						s = 0x3002 + hankana2zenhira_table[n];
					} else {
						pc->status = 1;
						pc->cache = c;
						s = 0x3000 + hankana2zenhira_table[n];
					}
				} else {
					pc->status = 1;
					pc->cache = c;
					return c;
				}
			} else {
				if (pc->status) {
					n = (pc->cache - 0xff60) & 0x3f;
					pc->status = 0;
					(*pc->next_filter->filter_function)(0x3000 + hankana2zenhira_table[n], pc->next_filter);
				}
			}
		} else if ((mode & 0x100) && c >= 0xff61 && c <= 0xff9f) {	/* hankaku kana to zenkaku katakana */
			s = 0x3000 + hankana2zenkata_table[c - 0xff60];
		} else if ((mode & 0x200) && c >= 0xff61 && c <= 0xff9f) {	/* hankaku kana to zenkaku hirangana */
			s = 0x3000 + hankana2zenhira_table[c - 0xff60];
		}
	}

	if (mode & 0x3000) {	/* Zenkaku kana to hankaku kana */
		if ((mode & 0x1000) && c >= 0x30a1 && c <= 0x30f4) {	/* Zenkaku katakana to hankaku kana */
			n = c - 0x30a1;
			if (zenkana2hankana_table[n][1] != 0) {
				(*pc->next_filter->filter_function)(0xff00 + zenkana2hankana_table[n][0], pc->next_filter);
				s = 0xff00 + zenkana2hankana_table[n][1];
			} else {
				s = 0xff00 + zenkana2hankana_table[n][0];
			}
		} else if ((mode & 0x2000) && c >= 0x3041 && c <= 0x3093) {	/* Zenkaku hirangana to hankaku kana */
			n = c - 0x3041;
			if (zenkana2hankana_table[n][1] != 0) {
				(*pc->next_filter->filter_function)(0xff00 + zenkana2hankana_table[n][0], pc->next_filter);
				s = 0xff00 + zenkana2hankana_table[n][1];
			} else {
				s = 0xff00 + zenkana2hankana_table[n][0];
			}
		} else if (c == 0x3001) {
			s = 0xff64;				/* HALFWIDTH IDEOGRAPHIC COMMA */
		} else if (c == 0x3002) {
			s = 0xff61;				/* HALFWIDTH IDEOGRAPHIC FULL STOP */
		} else if (c == 0x300c) {
			s = 0xff62;				/* HALFWIDTH LEFT CORNER BRACKET */
		} else if (c == 0x300d) {
			s = 0xff63;				/* HALFWIDTH RIGHT CORNER BRACKET */
		} else if (c == 0x309b) {
			s = 0xff9e;				/* HALFWIDTH KATAKANA VOICED SOUND MARK */
		} else if (c == 0x309c) {
			s = 0xff9f;				/* HALFWIDTH KATAKANA SEMI-VOICED SOUND MARK */
		} else if (c == 0x30fc) {
			s = 0xff70;				/* HALFWIDTH KATAKANA-HIRAGANA PROLONGED SOUND MARK */
		} else if (c == 0x30fb) {
			s = 0xff65;				/* HALFWIDTH KATAKANA MIDDLE DOT */
		}
	} else if (mode & 0x30000) { 
		if ((mode & 0x10000) && c >= 0x3041 && c <= 0x3093) {	/* Zenkaku hirangana to Zenkaku katakana */
			s = c + 0x60;
		} else if ((mode & 0x20000) && c >= 0x30a1 && c <= 0x30f3) {	/* Zenkaku katakana to Zenkaku hirangana */
			s = c - 0x60;
		}
	}

	if (mode & 0x100000) {	/* special ascii to symbol */
		if (c == 0x5c) {
			s = 0xffe5;				/* FULLWIDTH YEN SIGN */
		} else if (c == 0xa5) {		/* YEN SIGN */
			s = 0xffe5;				/* FULLWIDTH YEN SIGN */
		} else if (c == 0x7e) {
			s = 0xffe3;				/* FULLWIDTH MACRON */
		} else if (c == 0x203e) {	/* OVERLINE */
			s = 0xffe3;				/* FULLWIDTH MACRON */
		} else if (c == 0x27) {
			s = 0x2019;				/* RIGHT SINGLE QUOTATION MARK */
		} else if (c == 0x22) {
			s = 0x201d;				/* RIGHT DOUBLE QUOTATION MARK */
		}
	} else if (mode & 0x200000) {	/* special symbol to ascii */
		if (c == 0xffe5) {			/* FULLWIDTH YEN SIGN */
			s = 0x5c;
		} else if (c == 0xff3c) {	/* FULLWIDTH REVERSE SOLIDUS */
			s = 0x5c;
		} else if (c == 0xffe3) {	/* FULLWIDTH MACRON */
			s = 0x7e;
		} else if (c == 0x203e) {	/* OVERLINE */
			s = 0x7e;
		} else if (c == 0x2018) {	/* LEFT SINGLE QUOTATION MARK*/
			s = 0x27;
		} else if (c == 0x2019) {	/* RIGHT SINGLE QUOTATION MARK */
			s = 0x27;
		} else if (c == 0x201c) {	/* LEFT DOUBLE QUOTATION MARK */
			s = 0x22;
		} else if (c == 0x201d) {	/* RIGHT DOUBLE QUOTATION MARK */
			s = 0x22;
		}
	}

	if (mode & 0x400000) {	/* special ascii to symbol */
		if (c == 0x5c) {
			s = 0xff3c;				/* FULLWIDTH REVERSE SOLIDUS */
		} else if (c == 0x7e) {
			s = 0xff5e;				/* FULLWIDTH TILDE */
		} else if (c == 0x27) {
			s = 0xff07;				/* FULLWIDTH APOSTROPHE */
		} else if (c == 0x22) {
			s = 0xff02;				/* FULLWIDTH QUOTATION MARK */
		}
	} else if (mode & 0x800000) {	/* special symbol to ascii */
		if (c == 0xff3c) {			/* FULLWIDTH REVERSE SOLIDUS */
			s = 0x5c;
		} else if (c == 0xff5e) {	/* FULLWIDTH TILDE */
			s = 0x7e;
		} else if (c == 0xff07) {	/* FULLWIDTH APOSTROPHE */
			s = 0x27;
		} else if (c == 0xff02) {	/* FULLWIDTH QUOTATION MARK */
			s = 0x22;
		}
	}

	return (*pc->next_filter->filter_function)(s, pc->next_filter);
}

static int
collector_hantozen_flush(struct collector_hantozen_data *pc)
{
	int ret, n;

	ret = 0;
	if (pc->status) {
		n = (pc->cache - 0xff60) & 0x3f;
		if (pc->mode & 0x100) {	/* hankaku kana to zenkaku katakana */
			ret = (*pc->next_filter->filter_function)(0x3000 + hankana2zenkata_table[n], pc->next_filter);
		} else if (pc->mode & 0x200) {	/* hankaku kana to zenkaku hirangana */
			ret = (*pc->next_filter->filter_function)(0x3000 + hankana2zenhira_table[n], pc->next_filter);
		}
		pc->status = 0;
	}

	return ret;
}

mbfl_string *
mbfl_ja_jp_hantozen(
    mbfl_string *string,
    mbfl_string *result,
    int mode)
{
	int n;
	unsigned char *p;
	const mbfl_encoding *encoding;
	mbfl_memory_device device;
	struct collector_hantozen_data pc;
	mbfl_convert_filter *decoder;
	mbfl_convert_filter *encoder;

	/* initialize */
	if (string == NULL || result == NULL) {
		return NULL;
	}
	encoding = mbfl_no2encoding(string->no_encoding);
	if (encoding == NULL) {
		return NULL;
	}
	mbfl_memory_device_init(&device, string->len, 0);
	mbfl_string_init(result);
	result->no_language = string->no_language;
	result->no_encoding = string->no_encoding;
	decoder = mbfl_convert_filter_new(
	  mbfl_no_encoding_wchar,
	  string->no_encoding,
	  mbfl_memory_device_output, 0, &device);
	encoder = mbfl_convert_filter_new(
	  string->no_encoding,
	  mbfl_no_encoding_wchar,
	  collector_hantozen, 0, &pc);
	if (decoder == NULL || encoder == NULL) {
		mbfl_convert_filter_delete(encoder);
		mbfl_convert_filter_delete(decoder);
		return NULL;
	}
	pc.next_filter = decoder;
	pc.mode = mode;
	pc.status = 0;
	pc.cache = 0;

	/* feed data */
	p = string->val;
	n = string->len;
	if (p != NULL) {
		while (n > 0) {
			if ((*encoder->filter_function)(*p++, encoder) < 0) {
				break;
			}
			n--;
		}
	}

	mbfl_convert_filter_flush(encoder);
	collector_hantozen_flush(&pc);
	mbfl_convert_filter_flush(decoder);
	result = mbfl_memory_device_result(&device, result);
	mbfl_convert_filter_delete(encoder);
	mbfl_convert_filter_delete(decoder);

	return result;
}


/*
 *  MIME header encode
 */
struct mime_header_encoder_data {
	mbfl_convert_filter *conv1_filter;
	mbfl_convert_filter *block_filter;
	mbfl_convert_filter *conv2_filter;
	mbfl_convert_filter *conv2_filter_backup;
	mbfl_convert_filter *encod_filter;
	mbfl_convert_filter *encod_filter_backup;
	mbfl_memory_device outdev;
	mbfl_memory_device tmpdev;
	int status1;
	int status2;
	int prevpos;
	int linehead;
	int firstindent;
	int encnamelen;
	int lwsplen;
	char encname[128];
	char lwsp[16];
};

static int
mime_header_encoder_block_collector(int c, void *data)
{
	int n;
	struct mime_header_encoder_data *pe = (struct mime_header_encoder_data *)data;

	switch (pe->status2) {
	case 1:	/* encoded word */
		pe->prevpos = pe->outdev.pos;
		mbfl_convert_filter_copy(pe->conv2_filter, pe->conv2_filter_backup);
		mbfl_convert_filter_copy(pe->encod_filter, pe->encod_filter_backup);
		(*pe->conv2_filter->filter_function)(c, pe->conv2_filter);
		(*pe->conv2_filter->filter_flush)(pe->conv2_filter);
		(*pe->encod_filter->filter_flush)(pe->encod_filter);
		n = pe->outdev.pos - pe->linehead + pe->firstindent;
		pe->outdev.pos = pe->prevpos;
		mbfl_convert_filter_copy(pe->conv2_filter_backup, pe->conv2_filter);
		mbfl_convert_filter_copy(pe->encod_filter_backup, pe->encod_filter);
		if (n >= 74) {
			(*pe->conv2_filter->filter_flush)(pe->conv2_filter);
			(*pe->encod_filter->filter_flush)(pe->encod_filter);
			mbfl_memory_device_strncat(&pe->outdev, "\x3f\x3d", 2);	/* ?= */
			mbfl_memory_device_strncat(&pe->outdev, pe->lwsp, pe->lwsplen);
			pe->linehead = pe->outdev.pos;
			pe->firstindent = 0;
			mbfl_memory_device_strncat(&pe->outdev, pe->encname, pe->encnamelen);
			c = (*pe->conv2_filter->filter_function)(c, pe->conv2_filter);
		} else {
			c = (*pe->conv2_filter->filter_function)(c, pe->conv2_filter);
		}
		break;

	default:
		mbfl_memory_device_strncat(&pe->outdev, pe->encname, pe->encnamelen);
		c = (*pe->conv2_filter->filter_function)(c, pe->conv2_filter);
		pe->status2 = 1;
		break;
	}

	return c;
}

static int
mime_header_encoder_collector(int c, void *data)
{
	int n;
	struct mime_header_encoder_data *pe = (struct mime_header_encoder_data *)data;

	switch (pe->status1) {
	case 11:	/* encoded word */
		(*pe->block_filter->filter_function)(c, pe->block_filter);
		break;

	default:	/* ASCII */
		if (c >= 0x21 && c < 0x7f) {	/* ASCII exclude SPACE and CTLs */
			mbfl_memory_device_output(c, &pe->tmpdev);
			pe->status1 = 1;
		} else if (pe->status1 == 0 && c == 0x20) {	/* repeat SPACE */
			mbfl_memory_device_output(c, &pe->tmpdev);
		} else {
			if (pe->tmpdev.pos < 74 && c == 0x20) {
				n = pe->outdev.pos - pe->linehead + pe->tmpdev.pos + pe->firstindent;
				if (n > 74) {
					mbfl_memory_device_strncat(&pe->outdev, pe->lwsp, pe->lwsplen);		/* LWSP */
					pe->linehead = pe->outdev.pos;
					pe->firstindent = 0;
				} else if (pe->outdev.pos > 0) {
					mbfl_memory_device_output(0x20, &pe->outdev);
				}
				mbfl_memory_device_devcat(&pe->outdev, &pe->tmpdev);
				mbfl_memory_device_reset(&pe->tmpdev);
				pe->status1 = 0;
			} else {
				n = pe->outdev.pos - pe->linehead + pe->encnamelen + pe->firstindent;
				if (n > 60)  {
					mbfl_memory_device_strncat(&pe->outdev, pe->lwsp, pe->lwsplen);		/* LWSP */
					pe->linehead = pe->outdev.pos;
					pe->firstindent = 0;
				} else if (pe->outdev.pos > 0)  {
					mbfl_memory_device_output(0x20, &pe->outdev);
				}
				mbfl_convert_filter_devcat(pe->block_filter, &pe->tmpdev);
				mbfl_memory_device_reset(&pe->tmpdev);
				(*pe->block_filter->filter_function)(c, pe->block_filter);
				pe->status1 = 11;
			}
		}
		break;
	}

	return c;
}

mbfl_string *
mime_header_encoder_result(struct mime_header_encoder_data *pe, mbfl_string *result)
{
	if (pe->status1 >= 10) {
		(*pe->conv2_filter->filter_flush)(pe->conv2_filter);
		(*pe->encod_filter->filter_flush)(pe->encod_filter);
		mbfl_memory_device_strncat(&pe->outdev, "\x3f\x3d", 2);		/* ?= */
	} else if (pe->tmpdev.pos > 0) {
		if (pe->outdev.pos > 0) {
			if ((pe->outdev.pos - pe->linehead + pe->tmpdev.pos) > 74) {
				mbfl_memory_device_strncat(&pe->outdev, pe->lwsp, pe->lwsplen);
			} else {
				mbfl_memory_device_output(0x20, &pe->outdev);
			}
		}
		mbfl_memory_device_devcat(&pe->outdev, &pe->tmpdev);
	}
	mbfl_memory_device_reset(&pe->tmpdev);
	pe->prevpos = 0;
	pe->linehead = 0;
	pe->status1 = 0;
	pe->status2 = 0;

	return mbfl_memory_device_result(&pe->outdev, result);
}

struct mime_header_encoder_data*
mime_header_encoder_new(
    enum mbfl_no_encoding incode,
    enum mbfl_no_encoding outcode,
    enum mbfl_no_encoding transenc)
{
	int n;
	const char *s;
	const mbfl_encoding *outencoding;
	struct mime_header_encoder_data *pe;

	/* get output encoding and check MIME charset name */
	outencoding = mbfl_no2encoding(outcode);
	if (outencoding == NULL || outencoding->mime_name == NULL || outencoding->mime_name[0] == '\0') {
		return NULL;
	}

	pe = (struct mime_header_encoder_data*)mbfl_malloc(sizeof(struct mime_header_encoder_data));
	if (pe == NULL) {
		return NULL;
	}

	mbfl_memory_device_init(&pe->outdev, 0, 0);
	mbfl_memory_device_init(&pe->tmpdev, 0, 0);
	pe->prevpos = 0;
	pe->linehead = 0;
	pe->firstindent = 0;
	pe->status1 = 0;
	pe->status2 = 0;

	/* make the encoding description string  exp. "=?ISO-2022-JP?B?" */
	n = 0;
	pe->encname[n++] = 0x3d;
	pe->encname[n++] = 0x3f;
	s = outencoding->mime_name;
	while (*s) {
		pe->encname[n++] = *s++;
	}
	pe->encname[n++] = 0x3f;
	if (transenc == mbfl_no_encoding_qprint) {
		pe->encname[n++] = 0x51;
	} else {
		pe->encname[n++] = 0x42;
		transenc = mbfl_no_encoding_base64;
	}
	pe->encname[n++] = 0x3f;
	pe->encname[n] = '\0';
	pe->encnamelen = n;

	n = 0;
	pe->lwsp[n++] = 0x0d;
	pe->lwsp[n++] = 0x0a;
	pe->lwsp[n++] = 0x20;
	pe->lwsp[n] = '\0';
	pe->lwsplen = n;

	/* transfer encode filter */
	pe->encod_filter = mbfl_convert_filter_new(outcode, transenc, mbfl_memory_device_output, 0, &(pe->outdev));
	pe->encod_filter_backup = mbfl_convert_filter_new(outcode, transenc, mbfl_memory_device_output, 0, &(pe->outdev));

	/* Output code filter */
	pe->conv2_filter = mbfl_convert_filter_new(mbfl_no_encoding_wchar, outcode, mbfl_filter_output_pipe, 0, pe->encod_filter);
	pe->conv2_filter_backup = mbfl_convert_filter_new(mbfl_no_encoding_wchar, outcode, mbfl_filter_output_pipe, 0, pe->encod_filter);

	/* encoded block filter */
	pe->block_filter = mbfl_convert_filter_new(mbfl_no_encoding_wchar, mbfl_no_encoding_wchar, mime_header_encoder_block_collector, 0, pe);

	/* Input code filter */
	pe->conv1_filter = mbfl_convert_filter_new(incode, mbfl_no_encoding_wchar, mime_header_encoder_collector, 0, pe);

	if (pe->encod_filter == NULL ||
	    pe->encod_filter_backup == NULL ||
	    pe->conv2_filter == NULL ||
	    pe->conv2_filter_backup == NULL ||
	    pe->conv1_filter == NULL) {
		mime_header_encoder_delete(pe);
		return NULL;
	}

	if (transenc == mbfl_no_encoding_qprint) {
		pe->encod_filter->status |= MBFL_QPRINT_STS_MIME_HEADER;
		pe->encod_filter_backup->status |= MBFL_QPRINT_STS_MIME_HEADER;
	} else {
		pe->encod_filter->status |= MBFL_BASE64_STS_MIME_HEADER;
		pe->encod_filter_backup->status |= MBFL_BASE64_STS_MIME_HEADER;
	}

	return pe;
}

void
mime_header_encoder_delete(struct mime_header_encoder_data *pe)
{
	if (pe) {
		mbfl_convert_filter_delete(pe->conv1_filter);
		mbfl_convert_filter_delete(pe->block_filter);
		mbfl_convert_filter_delete(pe->conv2_filter);
		mbfl_convert_filter_delete(pe->conv2_filter_backup);
		mbfl_convert_filter_delete(pe->encod_filter);
		mbfl_convert_filter_delete(pe->encod_filter_backup);
		mbfl_memory_device_clear(&pe->outdev);
		mbfl_memory_device_clear(&pe->tmpdev);
		mbfl_free((void*)pe);
	}
}

int
mime_header_encoder_feed(int c, struct mime_header_encoder_data *pe)
{
	return (*pe->conv1_filter->filter_function)(c, pe->conv1_filter);
}

mbfl_string *
mbfl_mime_header_encode(
    mbfl_string *string,
    mbfl_string *result,
    enum mbfl_no_encoding outcode,
    enum mbfl_no_encoding encoding,
    const char *linefeed,
    int indent)
{
	int n;
	unsigned char *p;
	struct mime_header_encoder_data *pe;

	mbfl_string_init(result);
	result->no_language = string->no_language;
	result->no_encoding = mbfl_no_encoding_ascii;

	pe = mime_header_encoder_new(string->no_encoding, outcode, encoding);
	if (pe == NULL) {
		return NULL;
	}

	if (linefeed != NULL) {
		n = 0;
		while (*linefeed && n < 8) {
			pe->lwsp[n++] = *linefeed++;
		}
		pe->lwsp[n++] = 0x20;
		pe->lwsp[n] = '\0';
		pe->lwsplen = n;
	}
	if (indent > 0 && indent < 74) {
		pe->firstindent = indent;
	}

	n = string->len;
	p = string->val;
	while (n > 0) {
		(*pe->conv1_filter->filter_function)(*p++, pe->conv1_filter);
		n--;
	}

	result = mime_header_encoder_result(pe, result);
	mime_header_encoder_delete(pe);

	return result;
}


/*
 *  MIME header decode
 */
struct mime_header_decoder_data {
	mbfl_convert_filter *deco_filter;
	mbfl_convert_filter *conv1_filter;
	mbfl_convert_filter *conv2_filter;
	mbfl_memory_device outdev;
	mbfl_memory_device tmpdev;
	int cspos;
	int status;
	enum mbfl_no_encoding encoding;
	enum mbfl_no_encoding incode;
	enum mbfl_no_encoding outcode;
};

static int
mime_header_decoder_collector(int c, void* data)
{
	const mbfl_encoding *encoding;
	struct mime_header_decoder_data *pd = (struct mime_header_decoder_data*)data;

	switch (pd->status) {
	case 1:
		if (c == 0x3f) {		/* ? */
			mbfl_memory_device_output(c, &pd->tmpdev);
			pd->cspos = pd->tmpdev.pos;
			pd->status = 2;
		} else {
			mbfl_convert_filter_devcat(pd->conv1_filter, &pd->tmpdev);
			mbfl_memory_device_reset(&pd->tmpdev);
			if (c == 0x3d) {		/* = */
				mbfl_memory_device_output(c, &pd->tmpdev);
			} else if (c == 0x0d || c == 0x0a) {	/* CR or LF */
				pd->status = 9;
			} else {
				(*pd->conv1_filter->filter_function)(c, pd->conv1_filter);
				pd->status = 0;
			}
		}
		break;
	case 2:		/* store charset string */
		if (c == 0x3f) {		/* ? */
			/* identify charset */
			mbfl_memory_device_output('\0', &pd->tmpdev);
			encoding = mbfl_name2encoding((const char *)&pd->tmpdev.buffer[pd->cspos]);
			if (encoding != NULL) {
				pd->incode = encoding->no_encoding;
				pd->status = 3;
			}
			mbfl_memory_device_unput(&pd->tmpdev);
			mbfl_memory_device_output(c, &pd->tmpdev);
		} else {
			mbfl_memory_device_output(c, &pd->tmpdev);
			if (pd->tmpdev.pos > 100) {		/* too long charset string */
				pd->status = 0;
			} else if (c == 0x0d || c == 0x0a) {	/* CR or LF */
				mbfl_memory_device_unput(&pd->tmpdev);
				pd->status = 9;
			}
			if (pd->status != 2) {
				mbfl_convert_filter_devcat(pd->conv1_filter, &pd->tmpdev);
				mbfl_memory_device_reset(&pd->tmpdev);
			}
		}
		break;
	case 3:		/* identify encoding */
		mbfl_memory_device_output(c, &pd->tmpdev);
		if (c == 0x42 || c == 0x62) {		/* 'B' or 'b' */
			pd->encoding = mbfl_no_encoding_base64;
			pd->status = 4;
		} else if (c == 0x51 || c == 0x71) {	/* 'Q' or 'q' */
			pd->encoding = mbfl_no_encoding_qprint;
			pd->status = 4;
		} else {
			if (c == 0x0d || c == 0x0a) {	/* CR or LF */
				mbfl_memory_device_unput(&pd->tmpdev);
				pd->status = 9;
			} else {
				pd->status = 0;
			}
			mbfl_convert_filter_devcat(pd->conv1_filter, &pd->tmpdev);
			mbfl_memory_device_reset(&pd->tmpdev);
		}
		break;
	case 4:		/* reset filter */
		mbfl_memory_device_output(c, &pd->tmpdev);
		if (c == 0x3f) {		/* ? */
			/* charset convert filter */
			mbfl_convert_filter_reset(pd->conv1_filter, pd->incode, mbfl_no_encoding_wchar);
			/* decode filter */
			mbfl_convert_filter_reset(pd->deco_filter, pd->encoding, mbfl_no_encoding_8bit);
			pd->status = 5;
		} else {
			if (c == 0x0d || c == 0x0a) {	/* CR or LF */
				mbfl_memory_device_unput(&pd->tmpdev);
				pd->status = 9;
			} else {
				pd->status = 0;
			}
			mbfl_convert_filter_devcat(pd->conv1_filter, &pd->tmpdev);
		}
		mbfl_memory_device_reset(&pd->tmpdev);
		break;
	case 5:		/* encoded block */
		if (c == 0x3f) {		/* ? */
			pd->status = 6;
		} else {
			(*pd->deco_filter->filter_function)(c, pd->deco_filter);
		}
		break;
	case 6:		/* check end position */
		if (c == 0x3d) {		/* = */
			/* flush and reset filter */
			(*pd->deco_filter->filter_flush)(pd->deco_filter);
			(*pd->conv1_filter->filter_flush)(pd->conv1_filter);
			mbfl_convert_filter_reset(pd->conv1_filter, mbfl_no_encoding_ascii, mbfl_no_encoding_wchar);
			pd->status = 7;
		} else {
			(*pd->deco_filter->filter_function)(0x3f, pd->deco_filter);
			if (c != 0x3f) {		/* ? */
				(*pd->deco_filter->filter_function)(c, pd->deco_filter);
				pd->status = 5;
			}
		}
		break;
	case 7:		/* after encoded block */
		if (c == 0x0d || c == 0x0a) {	/* CR LF */
			pd->status = 8;
		} else {
			mbfl_memory_device_output(c, &pd->tmpdev);
			if (c == 0x3d) {		/* = */
				pd->status = 1;
			} else if (c != 0x20 && c != 0x09) {		/* not space */
				mbfl_convert_filter_devcat(pd->conv1_filter, &pd->tmpdev);
				mbfl_memory_device_reset(&pd->tmpdev);
				pd->status = 0;
			}
		}
		break;
	case 8:		/* folding */
	case 9:		/* folding */
		if (c != 0x0d && c != 0x0a && c != 0x20 && c != 0x09) {
			if (c == 0x3d) {		/* = */
				if (pd->status == 8) {
					mbfl_memory_device_output(0x20, &pd->tmpdev);	/* SPACE */
				} else {
					(*pd->conv1_filter->filter_function)(0x20, pd->conv1_filter);
				}
				mbfl_memory_device_output(c, &pd->tmpdev);
				pd->status = 1;
			} else {
				mbfl_memory_device_output(0x20, &pd->tmpdev);
				mbfl_memory_device_output(c, &pd->tmpdev);
				mbfl_convert_filter_devcat(pd->conv1_filter, &pd->tmpdev);
				mbfl_memory_device_reset(&pd->tmpdev);
				pd->status = 0;
			}
		}
		break;
	default:		/* non encoded block */
		if (c == 0x0d || c == 0x0a) {	/* CR LF */
			pd->status = 9;
		} else if (c == 0x3d) {		/* = */
			mbfl_memory_device_output(c, &pd->tmpdev);
			pd->status = 1;
		} else {
			(*pd->conv1_filter->filter_function)(c, pd->conv1_filter);
		}
		break;
	}

	return c;
}

mbfl_string *
mime_header_decoder_result(struct mime_header_decoder_data *pd, mbfl_string *result)
{
	switch (pd->status) {
	case 1:
	case 2:
	case 3:
	case 4:
	case 7:
	case 8:
	case 9:
		mbfl_convert_filter_devcat(pd->conv1_filter, &pd->tmpdev);
		break;
	case 5:
	case 6:
		(*pd->deco_filter->filter_flush)(pd->deco_filter);
		(*pd->conv1_filter->filter_flush)(pd->conv1_filter);
		break;
	}
	(*pd->conv2_filter->filter_flush)(pd->conv2_filter);
	mbfl_memory_device_reset(&pd->tmpdev);
	pd->status = 0;

	return mbfl_memory_device_result(&pd->outdev, result);
}

struct mime_header_decoder_data*
mime_header_decoder_new(enum mbfl_no_encoding outcode)
{
	struct mime_header_decoder_data *pd;

	pd = (struct mime_header_decoder_data*)mbfl_malloc(sizeof(struct mime_header_decoder_data));
	if (pd == NULL) {
		return NULL;
	}

	mbfl_memory_device_init(&pd->outdev, 0, 0);
	mbfl_memory_device_init(&pd->tmpdev, 0, 0);
	pd->cspos = 0;
	pd->status = 0;
	pd->encoding = mbfl_no_encoding_pass;
	pd->incode = mbfl_no_encoding_ascii;
	pd->outcode = outcode;
	/* charset convert filter */
	pd->conv2_filter = mbfl_convert_filter_new(mbfl_no_encoding_wchar, pd->outcode, mbfl_memory_device_output, 0, &pd->outdev);
	pd->conv1_filter = mbfl_convert_filter_new(pd->incode, mbfl_no_encoding_wchar, mbfl_filter_output_pipe, 0, pd->conv2_filter);
	/* decode filter */
	pd->deco_filter = mbfl_convert_filter_new(pd->encoding, mbfl_no_encoding_8bit, mbfl_filter_output_pipe, 0, pd->conv1_filter);

	if (pd->conv1_filter == NULL || pd->conv2_filter == NULL || pd->deco_filter == NULL) {
		mime_header_decoder_delete(pd);
		return NULL;
	}

	return pd;
}

void
mime_header_decoder_delete(struct mime_header_decoder_data *pd)
{
	if (pd) {
		mbfl_convert_filter_delete(pd->conv2_filter);
		mbfl_convert_filter_delete(pd->conv1_filter);
		mbfl_convert_filter_delete(pd->deco_filter);
		mbfl_memory_device_clear(&pd->outdev);
		mbfl_memory_device_clear(&pd->tmpdev);
		mbfl_free((void*)pd);
	}
}

int
mime_header_decoder_feed(int c, struct mime_header_decoder_data *pd)
{
	return mime_header_decoder_collector(c, pd);
}

mbfl_string *
mbfl_mime_header_decode(
    mbfl_string *string,
    mbfl_string *result,
    enum mbfl_no_encoding outcode)
{
	int n;
	unsigned char *p;
	struct mime_header_decoder_data *pd;

	mbfl_string_init(result);
	result->no_language = string->no_language;
	result->no_encoding = outcode;

	pd = mime_header_decoder_new(outcode);
	if (pd == NULL) {
		return NULL;
	}

	/* feed data */
	n = string->len;
	p = string->val;
	while (n > 0) {
		mime_header_decoder_collector(*p++, pd);
		n--;
	}

	result = mime_header_decoder_result(pd, result);
	mime_header_decoder_delete(pd);

	return result;
}



/*
 *  convert HTML numeric entity
 */
struct collector_htmlnumericentity_data {
	mbfl_convert_filter *decoder;
	int status;
	int cache;
	int digit;
	int *convmap;
	int mapsize;
};

static int
collector_encode_htmlnumericentity(int c, void *data)
{
	struct collector_htmlnumericentity_data *pc = (struct collector_htmlnumericentity_data *)data;
	int f, n, s, r, d, size, *mapelm;

	size = pc->mapsize;
	f = 0;
	n = 0;
	while (n < size) {
		mapelm = &(pc->convmap[n*4]);
		if (c >= mapelm[0] && c <= mapelm[1]) {
			s = (c + mapelm[2]) & mapelm[3];
			if (s >= 0) {
				(*pc->decoder->filter_function)(0x26, pc->decoder);	/* '&' */
				(*pc->decoder->filter_function)(0x23, pc->decoder);	/* '#' */
				r = 100000000;
				s %= r;
				while (r > 0) {
					d = s/r;
					if (d || f) {
						f = 1;
						s %= r;
						(*pc->decoder->filter_function)(mbfl_hexchar_table[d], pc->decoder);
					}
					r /= 10;
				}
				if (!f) {
					f = 1;
					(*pc->decoder->filter_function)(mbfl_hexchar_table[0], pc->decoder);
				}
				(*pc->decoder->filter_function)(0x3b, pc->decoder);		/* ';' */
			}
		}
		if (f) {
			break;
		}
		n++;
	}
	if (!f) {
		(*pc->decoder->filter_function)(c, pc->decoder);
	}

	return c;
}

static int
collector_decode_htmlnumericentity(int c, void *data)
{
	struct collector_htmlnumericentity_data *pc = (struct collector_htmlnumericentity_data *)data;
	int f, n, s, r, d, size, *mapelm;

	switch (pc->status) {
	case 1:
		if (c == 0x23) {	/* '#' */
			pc->status = 2;
		} else {
			pc->status = 0;
			(*pc->decoder->filter_function)(0x26, pc->decoder);		/* '&' */
			(*pc->decoder->filter_function)(c, pc->decoder);
		}
		break;
	case 2:
		if (c >= 0x30 && c <= 0x39) {	/* '0' - '9' */
			pc->cache = c - 0x30;
			pc->status = 3;
			pc->digit = 1;
		} else {
			pc->status = 0;
			(*pc->decoder->filter_function)(0x26, pc->decoder);		/* '&' */
			(*pc->decoder->filter_function)(0x23, pc->decoder);		/* '#' */
			(*pc->decoder->filter_function)(c, pc->decoder);
		}
		break;
	case 3:
		s = 0;
		f = 0;
		if (c >= 0x30 && c <= 0x39) {	/* '0' - '9' */
			if (pc->digit > 9) {
				pc->status = 0;
				s = pc->cache;
				f = 1;
			} else {
				s = pc->cache*10 + c - 0x30;
				pc->cache = s;
				pc->digit++;
			}
		} else {
			pc->status = 0;
			s = pc->cache;
			f = 1;
			n = 0;
			size = pc->mapsize;
			while (n < size) {
				mapelm = &(pc->convmap[n*4]);
				d = s - mapelm[2];
				if (d >= mapelm[0] && d <= mapelm[1]) {
					f = 0;
					(*pc->decoder->filter_function)(d, pc->decoder);
					if (c != 0x3b) {	/* ';' */
						(*pc->decoder->filter_function)(c, pc->decoder);
					}
					break;
				}
				n++;
			}
		}
		if (f) {
			(*pc->decoder->filter_function)(0x26, pc->decoder);		/* '&' */
			(*pc->decoder->filter_function)(0x23, pc->decoder);		/* '#' */
			r = 1;
			n = pc->digit;
			while (n > 0) {
				r *= 10;
				n--;
			}
			s %= r;
			r /= 10;
			while (r > 0) {
				d = s/r;
				s %= r;
				r /= 10;
				(*pc->decoder->filter_function)(mbfl_hexchar_table[d], pc->decoder);
			}
			(*pc->decoder->filter_function)(c, pc->decoder);
		}
		break;
	default:
		if (c == 0x26) {	/* '&' */
			pc->status = 1;
		} else {
			(*pc->decoder->filter_function)(c, pc->decoder);
		}
		break;
	}

	return c;
}

mbfl_string *
mbfl_html_numeric_entity(
    mbfl_string *string,
    mbfl_string *result,
    int *convmap,
    int mapsize,
    int type)
{
	struct collector_htmlnumericentity_data pc;
	mbfl_memory_device device;
	mbfl_convert_filter *encoder;
	int n;
	unsigned char *p;

	if (string == NULL || result == NULL) {
		return NULL;
	}
	mbfl_string_init(result);
	result->no_language = string->no_language;
	result->no_encoding = string->no_encoding;
	mbfl_memory_device_init(&device, string->len, 0);

	/* output code filter */
	pc.decoder = mbfl_convert_filter_new(
	    mbfl_no_encoding_wchar,
	    string->no_encoding,
	    mbfl_memory_device_output, 0, &device);
	/* wchar filter */
	if (type == 0) {
		encoder = mbfl_convert_filter_new(
		    string->no_encoding,
		    mbfl_no_encoding_wchar,
		    collector_encode_htmlnumericentity, 0, &pc);
	} else {
		encoder = mbfl_convert_filter_new(
		    string->no_encoding,
		    mbfl_no_encoding_wchar,
		    collector_decode_htmlnumericentity, 0, &pc);
	}
	if (pc.decoder == NULL || encoder == NULL) {
		mbfl_convert_filter_delete(encoder);
		mbfl_convert_filter_delete(pc.decoder);
		return NULL;
	}
	pc.status = 0;
	pc.cache = 0;
	pc.digit = 0;
	pc.convmap = convmap;
	pc.mapsize = mapsize;

	/* feed data */
	p = string->val;
	n = string->len;
	if (p != NULL) {
		while (n > 0) {
			if ((*encoder->filter_function)(*p++, encoder) < 0) {
				break;
			}
			n--;
		}
	}
	mbfl_convert_filter_flush(encoder);
	mbfl_convert_filter_flush(pc.decoder);
	result = mbfl_memory_device_result(&device, result);
	mbfl_convert_filter_delete(encoder);
	mbfl_convert_filter_delete(pc.decoder);

	return result;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
