/* =====================================================================
 * Author: Ignat Korchagin <ignat.korchagin@gmail.com>
 * This file is distributed under the same license as OpenSSL
 ==================================================================== */
#include "dstu.h"
#include "dstu_asn1.h"

static const ASN1_TEMPLATE DSTU_Pentanomial_seq_tt[] = {
	{
		.flags = 0,
		.tag = 0,
		.offset = offsetof(DSTU_Pentanomial, k),
		.field_name = "k",
		.item = &ASN1_INTEGER_it
	},
	{
		.flags = 0,
		.tag = 0,
		.offset = offsetof(DSTU_Pentanomial, j),
		.field_name = "j",
		.item = &ASN1_INTEGER_it
	},
	{
		.flags = 0,
		.tag = 0,
		.offset = offsetof(DSTU_Pentanomial, l),
		.field_name = "l",
		.item = &ASN1_INTEGER_it
	},
};

const ASN1_ITEM DSTU_Pentanomial_it = {
	ASN1_ITYPE_SEQUENCE,
	V_ASN1_SEQUENCE,
	DSTU_Pentanomial_seq_tt,
	sizeof(DSTU_Pentanomial_seq_tt) / sizeof(ASN1_TEMPLATE),
	NULL,
	sizeof(DSTU_Pentanomial),
	"DSTU_Pentanomial"
};

DSTU_Pentanomial *d2i_DSTU_Pentanomial(DSTU_Pentanomial **a, const unsigned char **in, long len)
{
	return (DSTU_Pentanomial *)ASN1_item_d2i((ASN1_VALUE **)a, in, len, (&(DSTU_Pentanomial_it)));
}

int i2d_DSTU_Pentanomial(DSTU_Pentanomial *a, unsigned char **out)
{
	return ASN1_item_i2d((ASN1_VALUE *)a, out, (&(DSTU_Pentanomial_it)));
}

DSTU_Pentanomial *DSTU_Pentanomial_new(void)
{
	return (DSTU_Pentanomial *)ASN1_item_new((&(DSTU_Pentanomial_it)));
}

void DSTU_Pentanomial_free(DSTU_Pentanomial *a)
{
	ASN1_item_free((ASN1_VALUE *)a, (&(DSTU_Pentanomial_it)));
}

static const ASN1_TEMPLATE DSTU_Polynomial_ch_tt[] = {
	{
		.flags = 0,
		.tag = 0,
		.offset = offsetof(DSTU_Polynomial, poly.k),
		.field_name = "poly.k",
		.item = &ASN1_INTEGER_it
	},
	{
		.flags = 0,
		.tag = 0,
		.offset = offsetof(DSTU_Polynomial, poly.pentanomial),
		.field_name = "poly.pentanomial",
		.item = &DSTU_Pentanomial_it
	}
};

const ASN1_ITEM DSTU_Polynomial_it = {
	ASN1_ITYPE_CHOICE,
	offsetof(DSTU_Polynomial, type),
	DSTU_Polynomial_ch_tt,
	sizeof(DSTU_Polynomial_ch_tt) / sizeof(ASN1_TEMPLATE),
	NULL,
	sizeof(DSTU_Polynomial),
	"DSTU_Polynomial"
};

DSTU_Polynomial *d2i_DSTU_Polynomial(DSTU_Polynomial **a, const unsigned char **in, long len)
{
	return (DSTU_Polynomial *)ASN1_item_d2i((ASN1_VALUE **)a, in, len, (&(DSTU_Polynomial_it)));
}

int i2d_DSTU_Polynomial(DSTU_Polynomial *a, unsigned char **out)
{
	return ASN1_item_i2d((ASN1_VALUE *)a, out, (&(DSTU_Polynomial_it)));\
}

DSTU_Polynomial *DSTU_Polynomial_new(void)
{
	return (DSTU_Polynomial *)ASN1_item_new((&(DSTU_Polynomial_it)));
}

void DSTU_Polynomial_free(DSTU_Polynomial *a)
{
	ASN1_item_free((ASN1_VALUE *)a, (&(DSTU_Polynomial_it)));
}

static const ASN1_TEMPLATE DSTU_BinaryField_seq_tt[] = {
	{
		.flags = 0,
		.tag = 0,
		.offset = offsetof(DSTU_BinaryField, m),
		.field_name = "m",
		.item = &ASN1_INTEGER_it
	},
	{
		.flags = 0,
		.tag = 0,
		.offset = offsetof(DSTU_BinaryField, poly),
		.field_name = "poly",
		.item = &DSTU_Polynomial_it
	}
};

const ASN1_ITEM DSTU_BinaryField_it = {
	ASN1_ITYPE_SEQUENCE,
	V_ASN1_SEQUENCE,
	DSTU_BinaryField_seq_tt,
	sizeof(DSTU_BinaryField_seq_tt) / sizeof(ASN1_TEMPLATE),
	NULL,
	sizeof(DSTU_BinaryField),
	"DSTU_BinaryField"
};

DSTU_BinaryField *d2i_DSTU_BinaryField(DSTU_BinaryField **a, const unsigned char **in, long len)
{
	return (DSTU_BinaryField *)ASN1_item_d2i((ASN1_VALUE **)a, in, len, (&(DSTU_BinaryField_it)));
}

int i2d_DSTU_BinaryField(DSTU_BinaryField *a, unsigned char **out)
{
	return ASN1_item_i2d((ASN1_VALUE *)a, out, (&(DSTU_BinaryField_it)));
}

DSTU_BinaryField *DSTU_BinaryField_new(void)
{
	return (DSTU_BinaryField *)ASN1_item_new((&(DSTU_BinaryField_it)));
}

void DSTU_BinaryField_free(DSTU_BinaryField *a)
{
	ASN1_item_free((ASN1_VALUE *)a, (&(DSTU_BinaryField_it)));
}

static const ASN1_TEMPLATE DSTU_CustomCurveSpec_seq_tt[] = {
	{
		.flags = 0,
		.tag = 0,
		.offset = offsetof(DSTU_CustomCurveSpec, field),
		.field_name = "field",
		.item = &DSTU_BinaryField_it
	},
	{
		.flags = 0,
		.tag = 0,
		.offset = offsetof(DSTU_CustomCurveSpec, a),
		.field_name = "a",
		.item = &ASN1_INTEGER_it
	},
	{
		.flags = 0,
		.tag = 0,
		.offset = offsetof(DSTU_CustomCurveSpec, b),
		.field_name = "b",
		.item = &ASN1_OCTET_STRING_it
	},
	{
		.flags = 0,
		.tag = 0,
		.offset = offsetof(DSTU_CustomCurveSpec, n),
		.field_name = "n",
		.item = &ASN1_INTEGER_it
	},
	{
		.flags = 0,
		.tag = 0,
		.offset = offsetof(DSTU_CustomCurveSpec, bp),
		.field_name = "bp",
		.item = &ASN1_OCTET_STRING_it
	}
};

const ASN1_ITEM DSTU_CustomCurveSpec_it = {
	ASN1_ITYPE_SEQUENCE,
	V_ASN1_SEQUENCE,
	DSTU_CustomCurveSpec_seq_tt,
	sizeof(DSTU_CustomCurveSpec_seq_tt) / sizeof(ASN1_TEMPLATE),
	NULL,
	sizeof(DSTU_CustomCurveSpec),
	"DSTU_CustomCurveSpec"
};

DSTU_CustomCurveSpec *d2i_DSTU_CustomCurveSpec(DSTU_CustomCurveSpec **a, const unsigned char **in, long len)
{
	return (DSTU_CustomCurveSpec *)ASN1_item_d2i((ASN1_VALUE **)a, in, len, (&(DSTU_CustomCurveSpec_it)));
}

int i2d_DSTU_CustomCurveSpec(DSTU_CustomCurveSpec *a, unsigned char **out)
{
	return ASN1_item_i2d((ASN1_VALUE *)a, out, (&(DSTU_CustomCurveSpec_it)));
}

DSTU_CustomCurveSpec *DSTU_CustomCurveSpec_new(void)
{
	return (DSTU_CustomCurveSpec *)ASN1_item_new((&(DSTU_CustomCurveSpec_it)));
}

void DSTU_CustomCurveSpec_free(DSTU_CustomCurveSpec *a)
{
	ASN1_item_free((ASN1_VALUE *)a, (&(DSTU_CustomCurveSpec_it)));
}

static const ASN1_TEMPLATE DSTU_CurveSpec_ch_tt[] = {
	{
		.flags = 0,
		.tag = 0,
		.offset = offsetof(DSTU_CurveSpec, curve.named_curve),
		.field_name = "curve.named_curve",
		.item = &ASN1_OBJECT_it
	},
	{
		.flags = 0,
		.tag = 0,
		.offset = offsetof(DSTU_CurveSpec, curve.custom_curve),
		.field_name = "curve.custom_curve",
		.item = &DSTU_CustomCurveSpec_it
	}
};

const ASN1_ITEM DSTU_CurveSpec_it = {
	ASN1_ITYPE_CHOICE,
	offsetof(DSTU_CurveSpec, type),
	DSTU_CurveSpec_ch_tt,
	sizeof(DSTU_CurveSpec_ch_tt) / sizeof(ASN1_TEMPLATE),
	NULL,
	sizeof(DSTU_CurveSpec),
	"DSTU_CurveSpec"
};

DSTU_CurveSpec *d2i_DSTU_CurveSpec(DSTU_CurveSpec **a, const unsigned char **in, long len)
{
	return (DSTU_CurveSpec *)ASN1_item_d2i((ASN1_VALUE **)a, in, len, (&(DSTU_CurveSpec_it)));
}

int i2d_DSTU_CurveSpec(DSTU_CurveSpec *a, unsigned char **out)
{
	return ASN1_item_i2d((ASN1_VALUE *)a, out, (&(DSTU_CurveSpec_it)));
}

DSTU_CurveSpec *DSTU_CurveSpec_new(void)
{
	return (DSTU_CurveSpec *)ASN1_item_new((&(DSTU_CurveSpec_it)));
}

void DSTU_CurveSpec_free(DSTU_CurveSpec *a)
{
	ASN1_item_free((ASN1_VALUE *)a, (&(DSTU_CurveSpec_it)));
}

static const ASN1_TEMPLATE DSTU_AlgorithmParameters_seq_tt[] = {
	{
		.flags = 0,
		.tag = 0,
		.offset = offsetof(DSTU_AlgorithmParameters, curve),
		.field_name = "curve",
		.item = &DSTU_CurveSpec_it
	},
	{
		.flags = ASN1_TFLG_OPTIONAL,
		.tag = 0,
		.offset = offsetof(DSTU_AlgorithmParameters, sbox),
		.field_name = "sbox",
		.item = &ASN1_OCTET_STRING_it
	}
};

const ASN1_ITEM DSTU_AlgorithmParameters_it = {
	ASN1_ITYPE_SEQUENCE,
	V_ASN1_SEQUENCE,
	DSTU_AlgorithmParameters_seq_tt,
	sizeof(DSTU_AlgorithmParameters_seq_tt) / sizeof(ASN1_TEMPLATE),
	NULL,
	sizeof(DSTU_AlgorithmParameters),
	"DSTU_AlgorithmParameters"
};

DSTU_AlgorithmParameters *d2i_DSTU_AlgorithmParameters(DSTU_AlgorithmParameters **a, const unsigned char **in, long len)
{
	return (DSTU_AlgorithmParameters *)ASN1_item_d2i((ASN1_VALUE **)a, in, len, (&(DSTU_AlgorithmParameters_it)));
}

int i2d_DSTU_AlgorithmParameters(DSTU_AlgorithmParameters *a, unsigned char **out)
{
	return ASN1_item_i2d((ASN1_VALUE *)a, out, (&(DSTU_AlgorithmParameters_it)));
}

DSTU_AlgorithmParameters *DSTU_AlgorithmParameters_new(void)
{
	return (DSTU_AlgorithmParameters *)ASN1_item_new((&(DSTU_AlgorithmParameters_it)));
}

void DSTU_AlgorithmParameters_free(DSTU_AlgorithmParameters *a)
{
	ASN1_item_free((ASN1_VALUE *)a, (&(DSTU_AlgorithmParameters_it)));
}
static const ASN1_TEMPLATE DSTU_Gost28147Parameters_seq_tt[] = {
	{
		.flags = 0,
		.tag = 0,
		.offset = offsetof(DSTU_Gost28147Parameters, iv),
		.field_name = "iv",
		.item = &ASN1_OCTET_STRING_it
	},
	{
		.flags = 0,
		.tag = 0,
		.offset = offsetof(DSTU_Gost28147Parameters, dke),
		.field_name = "dke",
		.item = &ASN1_OCTET_STRING_it
	}
};

const ASN1_ITEM DSTU_Gost28147Parameters_it = {
	ASN1_ITYPE_SEQUENCE,
	V_ASN1_SEQUENCE,
	DSTU_Gost28147Parameters_seq_tt,
	sizeof(DSTU_Gost28147Parameters_seq_tt) / sizeof(ASN1_TEMPLATE),
	NULL,
	sizeof(DSTU_Gost28147Parameters),
	"DSTU_Gost28147Parameters"
};

DSTU_Gost28147Parameters *d2i_DSTU_Gost28147Parameters(DSTU_Gost28147Parameters **a, const unsigned char **in, long len)
{
	return (DSTU_Gost28147Parameters *)ASN1_item_d2i((ASN1_VALUE **)a, in, len, (&(DSTU_Gost28147Parameters_it)));
}

int i2d_DSTU_Gost28147Parameters(DSTU_Gost28147Parameters *a, unsigned char **out)
{
	return ASN1_item_i2d((ASN1_VALUE *)a, out, (&(DSTU_Gost28147Parameters_it)));
}

DSTU_Gost28147Parameters *DSTU_Gost28147Parameters_new(void)
{
	return (DSTU_Gost28147Parameters *)ASN1_item_new((&(DSTU_Gost28147Parameters_it)));
}

void DSTU_Gost28147Parameters_free(DSTU_Gost28147Parameters *a)
{
	ASN1_item_free((ASN1_VALUE *)a, (&(DSTU_Gost28147Parameters_it)));
}
