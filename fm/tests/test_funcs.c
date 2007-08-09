#include "config.h"
#include <assert.h>
#include <fcntl.h>
#ifdef STDC_HEADERS
#include <stdlib.h>
#endif
#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif
#include <string.h>
#include "fm.h"
#include <stdio.h>

#include "test_funcs.h"

extern void init_written_data();

char *comment_array[] =
{"this is a comment in the file",
 "this is another comment in the file"};

first_rec rec1_array[13];

second_rec rec2_array[4];

third_rec rec3_array[5];

fourth_rec rec4;

fifth_rec rec5;

sixth_rec rec6_array[4];

nested_rec rec7_array[1];

later_rec rec8_array[3];

ninth_rec rec9_array[6];

string_array_rec string_array_array[6];

DeriveMsg derive;

multi_array_rec multi_array;

multi_array_rec2 multi_array2;

multi_array_rec fortran_array;

triangle_param triangle;

add_rec add_action_record;

char *first_xml = "\
<FirstRecord integer_attribute=<PBIO:data field_id=0> double_attribute=<PBIO:data field_id=1> character_attribute=<PBIO:data field_id=2>>\n";

char *string_xml = "\
<StringRecord integer_attribute=<PBIO:data field_id=0>>\n\
    <Short value=<PBIO:data field_id=1>>\n\
    <Long  value=<PBIO:data field_id=2>>\n\
    <String>\n\
       <PBIO:data field_id=3>\n\
    </String>\n\
    <Double value=<PBIO:data field_id=4>> \n\
    <Character value=<PBIO:data field_id=5>>\n\
</StringRecord>\n";

char *third_xml = "\
<TwoStringRecord integer_attribute=<PBIO:data field_name=\"integer field\">>\n\
    <Long  value=<PBIO:data field_name=\"long field\">>\n\
    <UnsignedInt  value=<PBIO:data field_name=\"uint field\">>\n\
    <UnsignedLong  value=<PBIO:data field_name=\"ulong field\">>\n\
    <String>\n\
       <PBIO:data field_id=4>\n\
    </String>\n\
    <Double value=<PBIO:data field_id=5>> \n\
    <StringTwo>\n\
       <PBIO:data field_name=\"string field2\">\n\
    </StringTwo>\n\
    <Character value=<PBIO:data field_id=7>>\n\
    <Enumeration value=<PBIO:data field_id=8>>\n\
</TwoStringRecord>\n";

char *fourth_xml = "\
<StaticArrayRecord  ifield=<PBIO:data field_name=ifield>>\n\
<IntArray><PBIO:array>\n\
	<ArrayElement>\n\
		<PBIO:array_data_mark  field_name=int_array>\n\
	</ArrayElement>\n\
</PBIO:array></IntArray>\n\
<DoubleArray><PBIO:array>\n\
	<DoubleElement>\n\
		<PBIO:array_data_mark  field_name=\"double field\">\n\
	</DoubleElement>\n\
</PBIO:array></DoubleArray>\n\
</StaticArrayRecord>\n";

char *embedded_xml = "\
<EmbeddedRecord  ifield=<PBIO:data field_name=ifield> \
string=\"<PBIO:data field_name=\"string field\">\" \
double=<PBIO:data field_name=\"dfield\">>\n";

char *struct_array_xml = "\
<StructuredArray>\n\
<PBIO:array><PBIO:array_data_mark field_name=earray></PBIO:array>\
</StructuredArray>\n";

char *var_array_xml = "\
<VariableLengthArrays>\n\
<VariableIntegerArray>\n\
<PBIO:array><PBIO:array_data_mark field_name=var_int_array> </PBIO:array>\
</VariableIntegerArray>\n\
<PBIO:array><PBIO:array_data_mark field_name=var_string_array></PBIO:array>\
</VariableLengthArrays>\n";

char *later_xml = "\
<LaterRecord>\n\
int=<PBIO:data field_name=\"integer field\"> str=<PBIO:data field_name=\"string field\"> dbl=<PBIO:data field_name=\"double field\">\n\
</LaterRecord>\n";

char *later2_xml = "\
<Later2Record>\n\
int=<PBIO:data field_name=\"integer field\"> str=<PBIO:data field_name=\"string field\"> dbl=<PBIO:data field_name=\"double field\">\n\
</Later2Record>\n";

char *nested_xml = "\
<NestedRecord>\n\
int=<PBIO:data field_name=\"integer field\"> \n\
<Nested Element>\n\
<PBIO:data field_name=\"nested record\">\n\
</Nested Element>\n\
<String><PBIO:data field_name=\"string field\"></String>\n\
</NestedRecord>\n";

char *event_xml = "<EventData len=<PBIO:data field_name=len>>\
<PBIO:array><PBIO:array_data_mark field_name=elem> </PBIO:array></EventData>\n";

char *event_vec_xml = "\
<EventVector num_blocks=<PBIO:data field_name=vec_length>>\n\
<PBIO:array><PBIO:array_data_mark field_name=eventv></PBIO:array>\
</EventVector>\n";

int
first_rec_eq(r1, r2)
first_rec *r1, *r2;
{
    if (r1->integer_field != r2->integer_field)
	return 0;
    if (r1->double_field != r2->double_field)
	return 0;
    if (r1->char_field != r2->char_field)
	return 0;
    return 1;
}

int
second_rec_eq(r1, r2)
second_rec *r1, *r2;
{
    if (r1->integer_field != r2->integer_field) {
	printf("integer_field 1 is %d (0x%x), integer_field 2 is %d (0x%x)\n",
	       r1->integer_field, r1->integer_field, 
	       r2->integer_field, r2->integer_field);
	return 0;
    }
    if (r1->short_field != r2->short_field) {
	printf("short_field 1 is %d (0x%x), short_field 2 is %d (0x%x)\n",
	       r1->short_field, r1->short_field, 
	       r2->short_field, r2->short_field);
	return 0;
    }
    if (r1->long_field != r2->long_field) {
	printf("long_field 1 is %ld (0x%lx), long_field 2 is %ld (0x%lx)\n",
	       r1->long_field, r1->long_field, 
	       r2->long_field, r2->long_field);
	return 0;
    }
    if ((r1->string != NULL) || (r2->string != NULL)) {
	if ((r1->string != NULL) && (r2->string != NULL)) {
	    if (strcmp(r1->string, r2->string) != 0) {
		printf("second_rec String 1 is %s, string 2 is %s\n", 
		       r1->string == NULL ? "(NULL)" : r1->string,
		       r2->string == NULL ? "(NULL)" : r2->string);
		return 0;
	    }
	} else {
	    printf("second_rec String 1 is %s, string 2 is %s\n", 
		   r1->string == NULL ? "(NULL)" : r1->string,
		   r2->string == NULL ? "(NULL)" : r2->string);
	    return 0;
	}
    }
    if (r1->double_field != r2->double_field)
	return 0;
    if (r1->char_field != r2->char_field)
	return 0;
    return 1;
}

int
third_rec_eq(r1, r2)
third_rec *r1, *r2;
{
    if (r1->integer_field != r2->integer_field) {
	printf("integer field 1 is %d (0x%x), integer_field 2 is %d (0x%x)\n",
	       r1->integer_field, r1->integer_field, r2->integer_field, r2->integer_field);
	return 0;
    }
    if (r1->long_field != r2->long_field) {
	printf("long field 1 is %ld (0x%lx), long_field 2 is %ld (0x%lx)\n",
	       r1->long_field, r1->long_field, r2->long_field, r2->long_field);
	return 0;
    }
    if (r1->uint_field != r2->uint_field) {
	printf("uint field 1 is %u (0x%x), uint_field 2 is %u (0x%x)\n",
	       r1->uint_field, r1->uint_field, r2->uint_field, r2->uint_field);
	return 0;
    }
    if (r1->ulong_field != r2->ulong_field) {
	printf("ulong field 1 is %lu (0x%lx), ulong_field 2 is %lu (0x%lx)\n",
	       r1->ulong_field, r1->ulong_field, r2->ulong_field, 
	       r2->ulong_field);
	return 0;
    }
    if ((r1->string != NULL) || (r2->string != NULL)) {
	if ((r1->string != NULL) && (r2->string != NULL)) {
	    if (strcmp(r1->string, r2->string) != 0) {
		printf("third_rec String 1 is %s, string 2 is %s\n", 
		       r1->string == NULL ? "(NULL)" : r1->string,
		       r2->string == NULL ? "(NULL)" : r2->string);
		return 0;
	    }
	} else {
	    printf("third_rec String 1 is %s, string 2 is %s\n", 
		   r1->string == NULL ? "(NULL)" : r1->string,
		   r2->string == NULL ? "(NULL)" : r2->string);
	    return 0;
	}
    }
    if (r1->double_field != r2->double_field) {
	printf("R1 double_field is %g , R2 is %g \n",
	       r1->double_field, r2->double_field);
	return 0;
    }
    if ((r1->string2 != NULL) || (r2->string2 != NULL)) {
	if ((r1->string2 != NULL) && (r2->string2 != NULL)) {
	    if (strcmp(r1->string2, r2->string2) != 0) {
		printf("third_rec String2 1 is %s, string2 2 is %s\n", 
		       r1->string2 == NULL ? "(NULL)" : r1->string2,
		       r2->string2 == NULL ? "(NULL)" : r2->string2);
		return 0;
	    }
	} else {
	    printf("third_rec String2 1 is %s, string2 2 is %s\n", 
		   r1->string2 == NULL ? "(NULL)" : r1->string2,
		   r2->string2 == NULL ? "(NULL)" : r2->string2);
	    return 0;
	}
    }
    if (r1->char_field != r2->char_field) {
	printf("char field 1 is %d (0x%x), char_field 2 is %d (0x%x)\n",
	       r1->char_field, r1->char_field, r2->char_field, r2->char_field);
	return 0;
    }
    if (r1->enum_field != r2->enum_field) {
	printf("enum field 1 is %d (0x%x), enum_field 2 is %d (0x%x)\n",
	       r1->enum_field, r1->enum_field, r2->enum_field, r2->enum_field);
	return 0;
    }
    return 1;
}

#define ARRAY_SIZE 14

int
fourth_rec_eq(r1, r2)
fourth_rec *r1, *r2;
{
    int i, j;
    if (r1->ifield != r2->ifield) {
	printf("Ifields not equal\n");
	return 0;
    }
    for (i = 0; i < ARRAY_SIZE; i++) {
	if (r1->int_array[i] != r2->int_array[i]) {
	    printf("Int_array element %d not equal\n", i);
	    return 0;
	}
    }
    for (i = 0; i < 2; i++) {
	for (j = 0; j < 2; j++) {
	    if (r1->double_array[i][j] != r2->double_array[i][j]) {
		printf("double_array[%d][%d] not equal\n", i, j);
		return 0;
	    }
	}
    }
    return 1;
}

int
emb_rec_eq(r1, r2)
embedded_rec *r1, *r2;
{
    if (r1->ifield != r2->ifield) {
	printf("ifield 1 is %d (%x), ifield 2 is %d (%x)\n",
	       r1->ifield, (unsigned)r1->ifield, 
	       r2->ifield, (unsigned)r2->ifield);
	return 0;
    }
    if ((r1->string != NULL) || (r2->string != NULL)) {
	if ((r1->string != NULL) && (r2->string != NULL)) {
	    if (strcmp(r1->string, r2->string) != 0) {
		printf("emb_rec String 1 is %s, string 2 is %s\n", 
		       r1->string == NULL ? "(NULL)" : r1->string,
		       r2->string == NULL ? "(NULL)" : r2->string);
		return 0;
	    }
	} else {
	    printf("emb_rec String 1 is %s, string 2 is %s\n", 
		   r1->string == NULL ? "(NULL)" : r1->string,
		   r2->string == NULL ? "(NULL)" : r2->string);
	    return 0;
	}
    }
    if (r1->dfield != r2->dfield) {
	printf("dfield 1 is %g, dfield 2 is %g\n", r1->dfield, r2->dfield);
	return 0;
    }
    return 1;
}

int
fifth_rec_eq(r1, r2)
fifth_rec *r1, *r2;
{
    int i;
    for (i = 0; i < 4; i++) {
	if (!emb_rec_eq(&r1->earray[i], &r2->earray[i])) {
	    printf("Failure was fifth_rec entry %d\n", i);
	    return 0;
	}
    }
    return 1;
}

int
sixth_rec_eq(r1, r2)
sixth_rec *r1, *r2;
{
    int i;
    if (r1->icount != r2->icount)
	return 0;
    if ((r1->string != NULL) || (r2->string != NULL)) {
	if ((r1->string != NULL) && (r2->string != NULL)) {
	    if (strcmp(r1->string, r2->string) != 0) {
		printf("Sixth_rec String 1 is %s, string 2 is %s\n", 
		       r1->string, r2->string);
		return 0;
	    }
	} else {
	    printf("Sixth_rec String 1 is %s, string 2 is %s\n", 
		   r1->string == NULL ? "(NULL)" : r1->string,
		   r2->string == NULL ? "(NULL)" : r2->string);
	    return 0;
	}
    }
    for (i = 0; i < r1->icount; i++) {
	if (r1->var_int_array[i] != r2->var_int_array[i]) {
	    printf("R1 var_int_array[%d] is %ld (0x%x), R2 is %ld (0x%x) \n",
		   i, (long)r1->var_int_array[i], (unsigned)r1->var_int_array[i],
		   (long)r2->var_int_array[i], (unsigned)r2->var_int_array[i]);
	    return 0;
	}
	if (r1->var_double_array[i] != r2->var_double_array[i]) {
	    printf("R1 var_double_array[%d] is %g (0x%lx), R2 is %g (0x%lx) \n",
		   i, r1->var_double_array[i], (long)r1->var_double_array[i],
		   r2->var_double_array[i], (long)r2->var_double_array[i]);
	    return 0;
	}
	if (!second_rec_eq(&r1->var_string_array[i], &r2->var_string_array[i])) {
	    printf("var string array element [%d] was different\n", i);
	    return 0;
	}
    }
    if (r1->dfield != r2->dfield) {
	printf("dfield 1 is %g, dfield 2 is %g\n", r1->dfield, r2->dfield);
	return 0;
    }
    return 1;
}

int
nested_rec_eq(r1, r2)
nested_rec *r1, *r2;
{
    if (r1->integer_field != r2->integer_field) {
	printf("nested, R1 integer = %d, R2 %d\n", r1->integer_field,
	       r2->integer_field);
	return 0;
    }
    if (!second_rec_eq(&r1->nested_rec, &r2->nested_rec)) {
	printf("Nested, second rec was not equal\n");
	return 0;
    }
    if ((r1->string != NULL) || (r2->string != NULL)) {
	if ((r1->string != NULL) && (r2->string != NULL)) {
	    if (strcmp(r1->string, r2->string) != 0) {
		printf("nested String 1 is %s, string 2 is %s\n", 
		       r1->string == NULL ? "(NULL)" : r1->string,
		       r2->string == NULL ? "(NULL)" : r2->string);
		return 0;
	    }
	} else {
	    printf("nested String 1 is %s, string 2 is %s\n", 
		   r1->string == NULL ? "(NULL)" : r1->string,
		   r2->string == NULL ? "(NULL)" : r2->string);
	    return 0;
	}
    }
    return 1;
}
    
int
later_rec_eq(r1, r2)
later_rec *r1, *r2;
{
    if (r1->integer_field != r2->integer_field)
	return 0;
    if ((r1->string != NULL) || (r2->string != NULL)) {
	if ((r1->string != NULL) && (r2->string != NULL)) {
	    if (strcmp(r1->string, r2->string) != 0)
		return 0;
	} else {
	    return 0;
	}
    }
    if (r1->double_field != r2->double_field)
	return 0;
    return 1;
}

int
ninth_rec_eq(r1, r2)
ninth_rec *r1, *r2;
{
    int i;
    if (r1->vec_length != r2->vec_length)
	return 0;
    for (i = 0; i < r1->vec_length; i++) {
	int j;
	if (r1->eventv[i].iov_len != r2->eventv[i].iov_len)
	    return 0;

	for (j = 0; j< r1->eventv[i].iov_len; j++) {
	    if (((char*)r1->eventv[i].iov_base)[j] != ((char*)r2->eventv[i].iov_base)[j]) 
		return 0;
	}
    }
    return 1;
}

int
string_array_eq(r1, r2)
string_array_rec *r1, *r2;
{
    int i;
    if (r1->array_len != r2->array_len)
	return 0;
    if ((r1->base_string != NULL) || (r2->base_string != NULL)) {
	if ((r1->base_string != NULL) && (r2->base_string != NULL)) {
	    if (strcmp(r1->base_string, r2->base_string) != 0)
		return 0;
	} else {
	    return 0;
	}
    }
    for (i = 0; i < r1->array_len; i++) {
	if ((r1->array[i] != NULL) || (r2->array[i] != NULL)) {
	    if ((r1->array[i] != NULL) && (r2->array[i] != NULL)) {
		if (strcmp(r1->array[i], r2->array[i]) != 0) {
		    printf("string array comparison failed for string entry %d, R1 is %s, R2 is %s\n", i,
			   r1->array[i] == NULL ? "(NULL)" : r1->array[i],
			   r2->array[i] == NULL ? "(NULL)" : r2->array[i]);
		    return 0;
		}
	    } else {
		printf("string array comparison failed for string entry %d, R1 is %s, R2 is %s\n", i,
		       r1->array[i] == NULL ? "(NULL)" : r1->array[i],
		       r2->array[i] == NULL ? "(NULL)" : r2->array[i]);
		return 0;
	    }
	}
    }
    return 1;
}

int
/* compares a single element */
fmfieldlist_eq(FMFieldList l1, FMFieldList l2)
{
    if ((l1->field_name != NULL) || (l2->field_name != NULL)) {
	if ((l1->field_name != NULL) && (l2->field_name != NULL)) {
	    if (strcmp(l1->field_name, l2->field_name) != 0) {
		printf("field list names differ\n");
		return 0;
	    }
	} else {
	    printf("Field list names differ, null non-null");
	    return 0;
	}
    }
    if ((l1->field_type != NULL) || (l2->field_type != NULL)) {
	if ((l1->field_type != NULL) && (l2->field_type != NULL)) {
	    if (strcmp(l1->field_type, l2->field_type) != 0) {
		printf("field list types differ, %s, %s\n", l1->field_type, l2->field_type);
		return 0;
	    }
	} else {
	    printf("Field list types differ, null non-null");
	    return 0;
	}
    }
    if (l1->field_size != l2->field_size) {
	printf("Field list sizes differ\n");
	return 0;
    }
    if (l1->field_offset != l2->field_offset) {
	printf("Field list offsets differ\n");
	return 0;
    }
    return 1;
}

int
deformatlist_eq(format_list_element *f1, format_list_element *f2)
{
    int i;
    if ((f1->format_name != NULL) || (f2->format_name != NULL)) {
	if ((f1->format_name != NULL) && (f2->format_name != NULL)) {
	    if (strcmp(f1->format_name, f2->format_name) != 0) {
		printf("format list names differ\n");
		return 0;
	    }
	} else {
	    printf("format list names differ, null non-null");
	    return 0;
	}
    }
    if (f1->field_list_len != f2->field_list_len) {
	printf("Field list lens differ\n");
	return 0;
    }
    for (i=0; i < f1->field_list_len ; i++) {
	if (!fmfieldlist_eq(&f1->field_list[i], &f2->field_list[i])) {
	    printf("Field list element %d of format list %s was different\n",
		   i, f1->format_name);
	    return 0;
	}
    }
    return 1;
}

int
derive_eq(DeriveMsgPtr d1, DeriveMsgPtr d2)
{
    int i;
    if ((d1->chan_str != NULL) || (d2->chan_str != NULL)) {
	if ((d1->chan_str != NULL) && (d2->chan_str != NULL)) {
	    if (strcmp(d1->chan_str, d2->chan_str) != 0) {
		printf("chan strs differ\n");
		return 0;
	    }
	} else {
	    printf("chan strs differ, null non-null");
	    return 0;
	}
    }
    if (d1->cond != d2->cond) {
	printf("conds differ\n");
	return 0;
    }
    if (d1->client_channel_id.len != d2->client_channel_id.len) {
	printf("id lens differ\n");
	return 0;
    }
    if (memcmp(d1->client_channel_id.channel, 
	       d2->client_channel_id.channel, 
	       d1->client_channel_id.len) != 0) {
	printf("ids differ\n");
	return 0;
    }
    if ((d1->client_contact_str != NULL) || (d2->client_contact_str != NULL)) {
	if ((d1->client_contact_str != NULL) && (d2->client_contact_str != NULL)) {
	    if (strcmp(d1->client_contact_str, d2->client_contact_str) != 0) {
		printf("client_contact_strs differ\n");
		return 0;
	    }
	} else {
	    printf("client_contact_strs differ, null non-null");
	    return 0;
	}
    }
    if ((d1->filter != NULL) || (d2->filter != NULL)) {
	if ((d1->filter != NULL) && (d2->filter != NULL)) {
	    if (strcmp(d1->filter, d2->filter) != 0) {
		printf("filters differ\n");
		return 0;
	    }
	} else {
	    printf("filters differ, null non-null");
	    return 0;
	}
    }
    if (d1->field_list_len != d2->field_list_len) {
	printf("Field list lens differ\n");
	return 0;
    }
    for (i=0; i < d1->field_list_len ; i++) {
	if (!fmfieldlist_eq(&d1->field_list[i], &d2->field_list[i])) {
	    printf("Field list element %d was different\n", i);
	    return 0;
	}
    }
    if (d1->format_list_len != d2->format_list_len) {
	printf("format list lens differ\n");
	return 0;
    }
    for (i=0; i < d1->format_list_len ; i++) {
	if (!deformatlist_eq(&d1->format_list[i], &d2->format_list[i])) {
	    printf("Format list element %d was different\n", i);
	    return 0;
	}
    }
    if (d1->init_data_len != d2->init_data_len) {
	printf("init data lens differ\n");
	return 0;
    }
    if (memcmp(d1->init_data_block, d2->init_data_block,
	       d1->init_data_len) != 0) {
	printf("init data differs\n");
	return 0;
    }
    return 1;
}
    
int
multi_array_eq(multi_array_rec *r1, multi_array_rec *r2)
{
    int i, j, k, l;
    if (r1->ifield != r2->ifield) {
	printf("ifield differs\n");
	return 0;
    }
    for (i = 0; i < 2; i++) {
	for (j = 0; j < 2; j++) {
	    for (k = 0; k < 2; k++) {
		for (l = 0; l < 2; l++) {
		    if (r1->double_array[i][j][k][l] != 
			r2->double_array[i][j][k][l]) {
			printf("Differ in double array element %d, %d, %d, %d\n",
			       i, j, k, l);
			return 0;
		    }
		}
	    }
	}
    }
    for (i = 0; i < r1->ifield; i++) {
	for (j = 0; j < 2; j++) {
	    if (r1->int_array[i][j] != r2->int_array[i][j]) {
		printf("Differ in int_array element %d, %d = %x, %x\n",
		       i, j, r1->int_array[i][j], r2->int_array[i][j]);
		return 0;
	    }
	}
    }
    for (i = 0; i < 2; i++) {
	for (j = 0; j < r1->ifield; j++) {
	    if (r1->int_array2[i][j] != r2->int_array2[i][j]) {
		printf("Differ in int_array2 element %d, %d\n",
		       i, j);
		return 0;
	    }
	}
    }
    for (i = 0; i < r1->ifield; i++) {
	for (j = 0; j < r1->ifield; j++) {
	    for (k = 0; k < r1->ifield; k++) {
		if (r1->int_array3[i][j][k] != r2->int_array3[i][j][k]) {
		    printf("Differ in int_array3 element %d, %d, %d  - r1=%d, r2=%d\n",
			   i, j, k, r1->int_array3[i][j][k], r2->int_array3[i][j][k]);
		    return 0;
		}
	    }
	}
    }
    return 1;
}

int
triangle_param_eq(triangle_param *r1, triangle_param *r2)
{
    int i;
    if (r1->corner1x != r2->corner1x) {
	printf("corner1x differs\n");
	return 0;
    }
    if (r1->corner1y != r2->corner1y) {
	printf("corner1y differs\n");
	return 0;
    }
    if (r1->corner1z != r2->corner1z) {
	printf("corner1z differs\n");
	return 0;
    }
    if (r1->corner2x != r2->corner2x) {
	printf("corner2x differs\n");
	return 0;
    }
    if (r1->corner2y != r2->corner2y) {
	printf("corner2y differs\n");
	return 0;
    }
    if (r1->corner2z != r2->corner2z) {
	printf("corner2z differs\n");
	return 0;
    }
    if (r1->compression_type != r2->compression_type) {
	printf("compression_type differs\n");
	return 0;
    }
    if (r1->codebook_size != r2->codebook_size) {
	printf("codebook_size differs\n");
	return 0;
    }
    for (i= 0; i < r1->codebook_size; i++) {
	if (r1->codebook_data[i] != r2->codebook_data[i]) {
	    printf("codebook_data[%d] differs\n", i);
	    return 0;
	}
    }	
    if (r1->timestamp != r2->timestamp) {
	printf("timestamp differs\n");
	return 0;
    }
    if (r1->nonce != r2->nonce) {
	printf("nonce differs\n");
	return 0;
    }
    if (r1->num_meshes != r2->num_meshes) {
	printf("num_meshes differs\n");
	return 0;
    }

    for (i=0; i < r1->num_meshes; i++) {
	int j;
	if (r1->mesh_data[i].id != r2->mesh_data[i].id) {
	    printf("mesh_data[%d].id differs\n", i);
	    return 0;
	}
	if (r1->mesh_data[i].data_size != r2->mesh_data[i].data_size) {
	    printf("mesh_data[%d].data_size differs\n", i);
	    return 0;
	}
	if (r1->mesh_data[i].old_data_size != r2->mesh_data[i].old_data_size) {
	    printf("mesh_data[%d].old_data_size differs\n", i);
	    return 0;
	}
	if (r1->mesh_data[i].compressed_size != r2->mesh_data[i].compressed_size) {
	    printf("mesh_data[%d].compressed_size differs\n", i);
	    return 0;
	}
	for (j = 0; j < r1->mesh_data[i].compressed_size; j++) {
	    if (r1->mesh_data[i].compressed_data[j] != r2->mesh_data[i].compressed_data[j]) {
		printf("mesh_data[%d].compressed_data[%d] differs\n", i, j);
		return 0;
	    }
	}
	if (r1->mesh_data[i].corner1x != r2->mesh_data[i].corner1x) {
	    printf("mesh_data[%d].corner1x differs\n", i);
	    return 0;
	}
	if (r1->mesh_data[i].corner1y != r2->mesh_data[i].corner1y) {
	    printf("mesh_data[%d].corner1y differs\n", i);
	    return 0;
	}
	if (r1->mesh_data[i].corner1z != r2->mesh_data[i].corner1z) {
	    printf("mesh_data[%d].corner1z differs\n", i);
	    return 0;
	}
	if (r1->mesh_data[i].corner2x != r2->mesh_data[i].corner2x) {
	    printf("mesh_data[%d].corner2x differs\n", i);
	    return 0;
	}
	if (r1->mesh_data[i].corner2y != r2->mesh_data[i].corner2y) {
	    printf("mesh_data[%d].corner2y differs\n", i);
	    return 0;
	}
	if (r1->mesh_data[i].corner2z != r2->mesh_data[i].corner2z) {
	    printf("mesh_data[%d].corner2z differs\n", i);
	    return 0;
	}
	if (r1->mesh_data[i].atom_type != r1->mesh_data[i].atom_type) {
	    printf("mesh_data[%d].corner1x differs\n", i);
	    return 0;
	}
    }
    return 1;
}

int
xml_format_list_eq(msg_format_list_element *f1, msg_format_list_element *f2)
{
    int i;
    if ((f1->format_name != NULL) || (f2->format_name != NULL)) {
	if ((f1->format_name != NULL) && (f2->format_name != NULL)) {
	    if (strcmp(f1->format_name, f2->format_name) != 0) {
		printf("format list names differ\n");
		return 0;
	    }
	} else {
	    printf("format list names differ, null non-null");
	    return 0;
	}
    }
    if ((f1->xml_markup != NULL) || (f2->xml_markup != NULL)) {
	if ((f1->xml_markup != NULL) && (f2->xml_markup != NULL)) {
	    if (strcmp(f1->xml_markup, f2->xml_markup) != 0) {
		printf("xml markups differ\n");
		return 0;
	    }
	} else {
	    printf("xml markups differ, null non-null");
	    return 0;
	}
    }
    if (f1->field_list_len != f2->field_list_len) {
	printf("Field list lens differ\n");
	return 0;
    }
    for (i=0; i < f1->field_list_len ; i++) {
	if (!fmfieldlist_eq(&f1->field_list[i], &f2->field_list[i])) {
	    printf("Field list element %d of format list %s was different\n",
		   i, f1->format_name);
	    return 0;
	}
    }
    return 1;
}

void
add_rec_dump(add_rec_ptr r)
{
    int i;
    printf("in_format_name %d \"%s\"\n", (char*)r->in_format_name - (char*)r,
	   r->in_format_name);
    printf("func_str %d \"%s\"\n", (char*)r->func_str - (char*)r, r->func_str);
    printf("out_formats %d \n", (char*)r->out_formats - (char*)r);
    for (i = 0; i < r->format_count; i++) {
	int j;
	printf("out[%d].format_name %d \"%s\"\n", i, (char*)r->out_formats[i].format_name - (char*)r, r->out_formats[i].format_name);
	if (r->out_formats[i].xml_markup != NULL)
	    printf("out[%d].xml_markup %d \"%s\"\n", i, (char*)r->out_formats[i].xml_markup - (char*)r, r->out_formats[i].xml_markup);
	printf("out[%d].field_list %d \n", i, (char*)r->out_formats[i].field_list - (char*)r);
	for (j = 0; j < r->out_formats[i].field_list_len; j++) {
	    printf("out[%d].field_list[%d].field_name %d \"%s\"\n", i, j, (char*)r->out_formats[i].field_list[j].field_name - (char*)r, r->out_formats[i].field_list[j].field_name);
	    printf("out[%d].field_list[%d].field_type %d \"%s\"\n", i, j, (char*)r->out_formats[i].field_list[j].field_type - (char*)r, r->out_formats[i].field_list[j].field_type);
	}
    }
}
	

int
add_rec_eq(add_rec_ptr r1, add_rec_ptr r2)
{
    int i;
    if (r1->action != r2->action) return 0;
    if ((r1->in_format_name != NULL) || (r2->in_format_name != NULL)) {
	if ((r1->in_format_name != NULL) && (r2->in_format_name != NULL)) {
	    if (strcmp(r1->in_format_name, r2->in_format_name) != 0) {
		printf("in_format_names differ\n");
		return 0;
	    }
	} else {
	    printf("in_format_names differ, null non-null");
	    return 0;
	}
    }
    if ((r1->func_str != NULL) || (r2->func_str != NULL)) {
	if ((r1->func_str != NULL) && (r2->func_str != NULL)) {
	    if (strcmp(r1->func_str, r2->func_str) != 0) {
		printf("func_strs differ, \"%s\", \"%s\"\n");
		return 0;
	    }
	} else {
	    printf("func_strs differ, null non-null");
	    return 0;
	}
    }
    if (r1->format_count != r2->format_count) return 0;
    for (i=0; i<r1->format_count; i++) {
	if (xml_format_list_eq(&r1->out_formats[i], &r2->out_formats[i]) == 0)
	    return 0;
    }
    return 1;
}

void
init_written_data()
{
    int i, j, k, l, index;
    memset((char *) &rec1_array, 0, sizeof(rec1_array));
    rec1_array[0].integer_field = 14;
    rec1_array[0].double_field = 2.717;
    rec1_array[0].char_field = 'A';
    rec1_array[1].integer_field = 17;
    rec1_array[1].double_field = rec1_array[0].double_field * 3.0;
    rec1_array[1].char_field = 'B';
    rec1_array[2].integer_field = rec1_array[1].integer_field * 2;
    rec1_array[2].double_field = rec1_array[1].double_field * 2.717;
    rec1_array[2].char_field = 'C';
    for (i = 0; i < 10; i++) {
	rec1_array[i + 3].integer_field = 2 * i * i;
	rec1_array[i + 3].double_field = 2.717 * (i * i);
	rec1_array[i + 3].char_field = 'D' + i;
    }

    memset((char *) &rec2_array[0], 0, sizeof(rec2_array));
    rec2_array[0].integer_field = 14;
    rec2_array[0].short_field = 27;
    rec2_array[0].long_field = 987234;
    rec2_array[0].string = "testing";
    rec2_array[0].double_field = 2.717;
    rec2_array[0].char_field = 'A';

    rec2_array[1].integer_field = 14;
    rec2_array[1].short_field = 27;
    rec2_array[1].long_field = 987234;
    rec2_array[1].string = NULL;
    rec2_array[1].double_field = 2.717;
    rec2_array[1].char_field = 'A';

    rec2_array[2].integer_field = 14;
    rec2_array[2].short_field = 27;
    rec2_array[2].long_field = 987234;
    rec2_array[2].string = NULL;
    rec2_array[2].double_field = 2.717;
    rec2_array[2].char_field = 'A';

    rec2_array[3].integer_field = 14;
    rec2_array[3].short_field = 27;
    rec2_array[3].long_field = 987234;
    rec2_array[3].string = "the end";
    rec2_array[3].double_field = 2.717;
    rec2_array[3].char_field = 'A';

    memset((char *) &rec3_array[0], 0, sizeof(rec3_array));
    rec3_array[0].integer_field = 14;
    rec3_array[0].long_field = 987234;
    rec3_array[0].uint_field = 0xf7e589ce;	/* = 4159015374 */
#if SIZEOF_LONG==64
    rec3_array[0].ulong_field = 0xf7e589ceec9dd130;
#else
    rec3_array[0].ulong_field = 0xec9dd130;	/* = 3969765680 */
#endif
    rec3_array[0].string = "testing";
    rec3_array[0].double_field = 2.717;
    rec3_array[0].string2 = "jambalaya";
    rec3_array[0].char_field = 'A';
    rec3_array[0].enum_field = Red_Stripe;

    rec3_array[1].integer_field = 14;
    rec3_array[1].long_field = 987234;
    rec3_array[1].uint_field = 0xf7e589ce;	/* = 4159015374 */
#if SIZEOF_LONG==64
    rec3_array[1].ulong_field = 0xf7e589ceec9dd130;
#else
    rec3_array[1].ulong_field = 0xec9dd130;	/* = 3969765680 */
#endif
    rec3_array[1].string = NULL;
    rec3_array[1].double_field = 2.717;
    rec3_array[1].string2 = "jambalaya";
    rec3_array[1].char_field = 'A';
    rec3_array[1].enum_field = Paulaner;

    rec3_array[2].integer_field = 14;
    rec3_array[2].long_field = 987234;
    rec3_array[2].uint_field = 0xf7e589ce;	/* = 4159015374 */
#if SIZEOF_LONG==64
    rec3_array[2].ulong_field = 0xf7e589ceec9dd130;
#else
    rec3_array[2].ulong_field = 0xec9dd130;	/* = 3969765680 */
#endif
    rec3_array[2].string = "testing";
    rec3_array[2].double_field = 2.717;
    rec3_array[2].string2 = NULL;
    rec3_array[2].char_field = 'A';
    rec3_array[2].enum_field = Pilsner;

    rec3_array[3].integer_field = 14;
    rec3_array[3].long_field = 987234;
    rec3_array[3].uint_field = 0xf7e589ce;	/* = 4159015374 */
#if SIZEOF_LONG==64
    rec3_array[3].ulong_field = 0xf7e589ceec9dd130;
#else
    rec3_array[3].ulong_field = 0xec9dd130;	/* = 3969765680 */
#endif
    rec3_array[3].string = NULL;
    rec3_array[3].double_field = 2.717;
    rec3_array[3].string2 = NULL;
    rec3_array[3].char_field = 'A';
    rec3_array[3].enum_field = Red_Stripe;

    rec3_array[4].integer_field = 14;
    rec3_array[4].long_field = 987234;
    rec3_array[4].uint_field = 0xf7e589ce;	/* = 4159015374 */
#if SIZEOF_LONG==64
    rec3_array[4].ulong_field = 0xf7e589ceec9dd130;
#else
    rec3_array[4].ulong_field = 0xec9dd130;	/* = 3969765680 */
#endif
    rec3_array[4].string = "testing";
    rec3_array[4].double_field = 2.717;
    rec3_array[4].string2 = "jambalaya";
    rec3_array[4].char_field = 'A';
    rec3_array[4].enum_field = Pilsner;

    memset((char *) &rec4, 0, sizeof(rec4));
    for (i = 0; i < ARRAY_SIZE; i++) {
	rec4.int_array[i] = 297 + i;
    }
    rec4.double_array[0][0] = 1.0;
    rec4.double_array[0][1] = 2.0;
    rec4.double_array[1][0] = 3.0;
    rec4.double_array[1][1] = 4.0;
    rec4.ifield = -rec4.int_array[ARRAY_SIZE - 1];

    memset((char *) &rec5, 0, sizeof(rec5));
    rec5.earray[0].dfield = 4.0;
    rec5.earray[0].ifield = 4;
    rec5.earray[0].string = "string20";
    rec5.earray[1].dfield = 3.0;
    rec5.earray[1].ifield = 3;
    rec5.earray[1].string = "string15";
    rec5.earray[2].dfield = 2.0;
    rec5.earray[2].ifield = 2;
    rec5.earray[2].string = "string10";
    rec5.earray[3].dfield = 1.0;
    rec5.earray[3].ifield = 1;
    rec5.earray[3].string = "string5";

    k = 0;
    for (i = 1; i < 20; i += 5) {
	rec6_array[k].string = malloc(10);
	memset(rec6_array[k].string, 0, 10);
	sprintf(rec6_array[k].string, "variant%d", i);
	rec6_array[k].icount = 2 * i;
	rec6_array[k].var_int_array = malloc(sizeof(((sixth_rec_ptr) 0)->var_int_array[0]) * rec6_array[k].icount);
	rec6_array[k].var_double_array = malloc(sizeof(double) * rec6_array[k].icount);
	rec6_array[k].var_string_array = malloc(sizeof(second_rec) * rec6_array[k].icount);
	for (j = 0; j < rec6_array[k].icount; j++) {
	    rec6_array[k].var_int_array[j] = 297 + j;
	    rec6_array[k].var_double_array[j] = 2.717 * j;
	    rec6_array[k].var_string_array[j].integer_field = 345 * j;
	    rec6_array[k].var_string_array[j].short_field = j;
	    rec6_array[k].var_string_array[j].long_field = 785 * j;
	    rec6_array[k].var_string_array[j].string = malloc(15);
	    memset(rec6_array[k].var_string_array[j].string, 0, 15);
	    sprintf(rec6_array[k].var_string_array[j].string,
		    "substring%d", j);
	    rec6_array[k].var_string_array[j].double_field = 3.1415 * j;
	    rec6_array[k].var_string_array[j].char_field = 'a' + 2 * j;
	}
	k++;
    }
    rec7_array[0].integer_field = 47;
    rec7_array[0].nested_rec.integer_field = 14;
    rec7_array[0].nested_rec.short_field = 27;
    rec7_array[0].nested_rec.long_field = 987234;
    rec7_array[0].nested_rec.string = "Another string";
    rec7_array[0].nested_rec.double_field = 2.717;
    rec7_array[0].nested_rec.char_field = 'A';
    rec7_array[0].string = "Yet another string";

    memset((char *) &rec8_array[0], 0, sizeof(rec8_array));
    rec8_array[0].integer_field = 9872346;
    rec8_array[0].string = "ABCD";
    rec8_array[0].double_field = 3.14159265358797323;

    rec8_array[1].integer_field = 23462346;
    rec8_array[1].string = "Efghij";
    rec8_array[1].double_field = 3.14159265358797323 * 2.0;

    rec8_array[2].integer_field = 2346987;
    rec8_array[2].string = "Klmn";
    rec8_array[2].double_field = 3.14159265358797323 * 3.0;

    index = 0;
    for (i = 1; i < 10; i += 2) {
        memset((char *) &rec9_array[index], 0, sizeof(ninth_rec));
	memset((char *) &string_array_array[index], 0, 
	       sizeof(string_array_rec));
	rec9_array[index].vec_length = i;
	rec9_array[index].eventv = malloc(sizeof(((ninth_rec_ptr) 0)->eventv[0]) * rec9_array[index].vec_length);

	string_array_array[index].array_len = i;
	string_array_array[index].array = malloc(sizeof(char*) * i);

	for (j = 0; j < rec9_array[index].vec_length; j++) {
	    int k;
	    rec9_array[index].eventv[j].iov_len = j + i;
	    rec9_array[index].eventv[j].iov_base = malloc(j + i);
	    string_array_array[index].array[j] = malloc(i + j + 2);

	    for (k=0; k<j+i; k++) {
		((char*)rec9_array[index].eventv[j].iov_base)[k] = 'A' + k + i/5;
		string_array_array[index].array[j][k] = 'a' + k + i;
	    }
	    string_array_array[index].array[j][k] = 0;
	}
	if ((i %4) == 1) {
	    string_array_array[index].base_string = NULL;
	} else {
	    string_array_array[index].base_string = strdup("Whoa there!");
	}
	index++;
    }
    derive.chan_str = "b013050800000000";
    derive.cond = 2; 
    derive.client_channel_id.len = 12; 
    derive.client_channel_id.channel = malloc(12);
    for(i=0; i < 12; i++) derive.client_channel_id.channel[i] = 2 * 1 + 1;
    derive.client_contact_str = "AAIAAEFQSUPEBc+CUFBJQ3yUAAA=";
    derive.filter = "{ return 1;}"; 
    derive.field_list_len = 3; 
    derive.field_list = malloc(3*sizeof(derive.field_list[0]));
    derive.field_list[0].field_name = "num_points"; 
    derive.field_list[0].field_type = "integer"; 
    derive.field_list[0].field_size = 4;  
    derive.field_list[0].field_offset = 0; 
    derive.field_list[1].field_name = "image_data";  
    derive.field_list[1].field_type = "PolygonPoints[num_points]"; 
    derive.field_list[1].field_size = 8;  
    derive.field_list[1].field_offset = 4; 
    derive.field_list[2].field_name = ""; 
    derive.field_list[2].field_type = ""; 
    derive.field_list[2].field_size = 0; 
    derive.field_list[2].field_offset = 0;
    derive.format_list_len = 2; 
    derive.format_list = malloc(2*sizeof(derive.format_list[0]));
    derive.format_list[0].format_name = "PipelinedPoint"; 
    derive.format_list[0].field_list_len = 7; 
    derive.format_list[0].field_list = 
      malloc(7*sizeof(derive.format_list[0].field_list[0]));
    derive.format_list[0].field_list[0].field_name = "x"; 
    derive.format_list[0].field_list[0].field_type = "integer"; 
    derive.format_list[0].field_list[0].field_size = 2; 
    derive.format_list[0].field_list[0].field_offset = 0;
    derive.format_list[0].field_list[1].field_name = "y"; 
    derive.format_list[0].field_list[1].field_type = "integer"; 
    derive.format_list[0].field_list[1].field_size = 2; 
    derive.format_list[0].field_list[1].field_offset = 2;
    derive.format_list[0].field_list[2].field_name = "z"; 
    derive.format_list[0].field_list[2].field_type = "integer"; 
    derive.format_list[0].field_list[2].field_size = 2; 
    derive.format_list[0].field_list[2].field_offset = 4; 
    derive.format_list[0].field_list[3].field_name = "r"; 
    derive.format_list[0].field_list[3].field_type = "integer"; 
    derive.format_list[0].field_list[3].field_size = 2; 
    derive.format_list[0].field_list[3].field_offset = 6;
    derive.format_list[0].field_list[4].field_name = "g"; 
    derive.format_list[0].field_list[4].field_type = "integer"; 
    derive.format_list[0].field_list[4].field_size = 2; 
    derive.format_list[0].field_list[4].field_offset = 8;
    derive.format_list[0].field_list[5].field_name = "b"; 
    derive.format_list[0].field_list[5].field_type = "integer"; 
    derive.format_list[0].field_list[5].field_size = 2; 
    derive.format_list[0].field_list[5].field_offset = 10;
    derive.format_list[0].field_list[6].field_name = ""; 
    derive.format_list[0].field_list[6].field_type = ""; 
    derive.format_list[0].field_list[6].field_size = 0; 
    derive.format_list[0].field_list[6].field_offset = 0; 
    derive.format_list[1].format_name = "PolygonPoints"; 
    derive.format_list[1].field_list_len = 3; 
    derive.format_list[1].field_list = 
      malloc(3*sizeof(derive.format_list[1].field_list[0]));
    derive.format_list[1].field_list[0].field_name = "num_points"; 
    derive.format_list[1].field_list[0].field_type = "integer"; 
    derive.format_list[1].field_list[0].field_size = 4; 
    derive.format_list[1].field_list[0].field_offset = 0;
    derive.format_list[1].field_list[1].field_name = "polygon_points"; 
    derive.format_list[1].field_list[1].field_type = "PipelinedPoint[num_points]"; 
    derive.format_list[1].field_list[1].field_size = 12; 
    derive.format_list[1].field_list[1].field_offset = 4;
    derive.format_list[1].field_list[2].field_name = ""; 
    derive.format_list[1].field_list[2].field_type = ""; 
    derive.format_list[1].field_list[2].field_size = 0; 
    derive.format_list[1].field_list[2].field_offset = 0;
    derive.init_data_block = NULL; 
    derive.init_data_len = 0;

    multi_array.ifield = 4;
    multi_array2.ifield = 4;
    for (i = 0; i < 2; i++) {
	for (j = 0; j < 2; j++) {
	    for (k = 0; k < 2; k++) {
		for (l = 0; l < 2; l++) {
		    multi_array.double_array[i][j][k][l] = 
			1000*i + 100*j + 10*k +l;
		    multi_array2.double_array[i][j][k][l] = 
			1000*i + 100*j + 10*k +l;
		}
	    }
	}
    }
    multi_array.int_array = malloc(2*4*sizeof(int));
    multi_array2.int_array = malloc(2*4*sizeof(int));
    for (i = 0; i < 4; i++) {
	for (j = 0; j < 2; j++) {
	    multi_array.int_array[i][j] = 1000*i + 100*j;
	}
    }
    multi_array.int_array2 = malloc(4*2*sizeof(int));
    multi_array2.int_array2 = malloc(4*2*sizeof(int));
    for (i = 0; i < 2; i++) {
	for (j = 0; j < 4; j++) {
	    multi_array.int_array2[i][j] = 1000*i + 100*j;
	    (*multi_array2.int_array)[i][j] = 1000*i + 100*j;
	}
    }
    for (i = 0; i < 4; i++) {
	for (j = 0; j < 2; j++) {
	    (*multi_array2.int_array2)[i][j] = 1000*i + 100*j;
	}
    }
    multi_array.int_array3 = malloc(4*4*4*sizeof(int));
    multi_array2.int_array3 = malloc(4*4*4*sizeof(int));
    for (i = 0; i < 4; i++) {
	for (j = 0; j < 4; j++) {
	    for (k = 0; k < 4; k++) {
		multi_array.int_array3[i][j][k] = 1000*i + 100*j + 10*k;
		(*multi_array2.int_array3)[i][j][k] = 1000*i + 100*j + 10*k;
	    }
	}
    }
    fortran_array.ifield = 4;
    for (i = 0; i < 2; i++) {
	for (j = 0; j < 2; j++) {
	    for (k = 0; k < 2; k++) {
		for (l = 0; l < 2; l++) {
		    fortran_array.double_array[l][k][j][i] = 
			1000*i + 100*j + 10*k +l;
		}
	    }
	}
    }
    fortran_array.int_array = malloc(2*4*sizeof(int));
    for (i = 0; i < 2; i++) {
	for (j = 0; j < 4; j++) {
	    (*((int (*)[4][2]) fortran_array.int_array))[j][i] = 1000*i + 100*j;
	}
    }
    fortran_array.int_array2 = malloc(4*2*sizeof(int));
    for (i = 0; i < 4; i++) {
	for (j = 0; j < 2; j++) {
	    (*((int (*)[2][4]) fortran_array.int_array2))[j][i] = 1000*i + 100*j;
	}
    }
    fortran_array.int_array3 = malloc(4*4*4*sizeof(int));
    for (i = 0; i < 4; i++) {
	for (j = 0; j < 4; j++) {
	    for (k = 0; k < 4; k++) {
		fortran_array.int_array3[k][j][i] = 1000*i + 100*j + 10*k;
	    }
	}
    }
    triangle.corner1x = triangle.corner1y = triangle.corner1z = 4;
    triangle.corner2x = triangle.corner2y = triangle.corner2z = 6;
    triangle.compression_type = 5;
    triangle.codebook_size = 10;
    triangle.codebook_data = malloc(triangle.codebook_size * sizeof(int));
    for (i=0; i< triangle.codebook_size; i++) {
	triangle.codebook_data[i] = i + 10;
    }
    triangle.timestamp = 12345;
    triangle.nonce = 5;
    triangle.num_meshes = 5;
    triangle.mesh_data = malloc(triangle.num_meshes * sizeof(triangle.mesh_data[0]));
    for (i=0; i < triangle.num_meshes; i++) {
	int j;
	triangle.mesh_data[i].id = i;
	triangle.mesh_data[i].data_size = 20;
	triangle.mesh_data[i].old_data_size = 20;
	triangle.mesh_data[i].compressed_size = 5;
	triangle.mesh_data[i].compressed_data = malloc(triangle.mesh_data[i].compressed_size * sizeof(int));
	for (j = 0; j < triangle.mesh_data[i].compressed_size; j++) {
	    triangle.mesh_data[i].compressed_data[j] = 2 * j + 40;
	}
	triangle.mesh_data[i].corner1x = triangle.mesh_data[i].corner1y = 
	    triangle.mesh_data[i].corner1z = i;
	triangle.mesh_data[i].corner2x = triangle.mesh_data[i].corner2y = 
	    triangle.mesh_data[i].corner2z = i+1;
	triangle.mesh_data[i].atom_type = 13;
    }
    add_action_record.action = 5;
    add_action_record.in_format_name = "my format";
    add_action_record.format_count = 2; 
    add_action_record.out_formats = malloc(2*sizeof(add_action_record.out_formats[0]));
    add_action_record.out_formats[0].format_name = "PipelinedPoint"; 
    add_action_record.out_formats[0].xml_markup = NULL; 
    add_action_record.out_formats[0].field_list_len = 7; 
    add_action_record.out_formats[0].field_list = 
      malloc(7*sizeof(add_action_record.out_formats[0].field_list[0]));
    add_action_record.out_formats[0].field_list[0].field_name = "x"; 
    add_action_record.out_formats[0].field_list[0].field_type = "integer"; 
    add_action_record.out_formats[0].field_list[0].field_size = 2; 
    add_action_record.out_formats[0].field_list[0].field_offset = 0;
    add_action_record.out_formats[0].field_list[1].field_name = "y"; 
    add_action_record.out_formats[0].field_list[1].field_type = "integer"; 
    add_action_record.out_formats[0].field_list[1].field_size = 2; 
    add_action_record.out_formats[0].field_list[1].field_offset = 2;
    add_action_record.out_formats[0].field_list[2].field_name = "z"; 
    add_action_record.out_formats[0].field_list[2].field_type = "integer"; 
    add_action_record.out_formats[0].field_list[2].field_size = 2; 
    add_action_record.out_formats[0].field_list[2].field_offset = 4; 
    add_action_record.out_formats[0].field_list[3].field_name = "r"; 
    add_action_record.out_formats[0].field_list[3].field_type = "integer"; 
    add_action_record.out_formats[0].field_list[3].field_size = 2; 
    add_action_record.out_formats[0].field_list[3].field_offset = 6;
    add_action_record.out_formats[0].field_list[4].field_name = "g"; 
    add_action_record.out_formats[0].field_list[4].field_type = "integer"; 
    add_action_record.out_formats[0].field_list[4].field_size = 2; 
    add_action_record.out_formats[0].field_list[4].field_offset = 8;
    add_action_record.out_formats[0].field_list[5].field_name = "b"; 
    add_action_record.out_formats[0].field_list[5].field_type = "integer"; 
    add_action_record.out_formats[0].field_list[5].field_size = 2; 
    add_action_record.out_formats[0].field_list[5].field_offset = 10;
    add_action_record.out_formats[0].field_list[6].field_name = ""; 
    add_action_record.out_formats[0].field_list[6].field_type = ""; 
    add_action_record.out_formats[0].field_list[6].field_size = 0; 
    add_action_record.out_formats[0].field_list[6].field_offset = 0; 
    add_action_record.out_formats[1].format_name = "PolygonPoints"; 
    add_action_record.out_formats[1].xml_markup = NULL; 
    add_action_record.out_formats[1].field_list_len = 3; 
    add_action_record.out_formats[1].field_list = 
      malloc(3*sizeof(add_action_record.out_formats[1].field_list[0]));
    add_action_record.out_formats[1].field_list[0].field_name = "num_points"; 
    add_action_record.out_formats[1].field_list[0].field_type = "integer"; 
    add_action_record.out_formats[1].field_list[0].field_size = 4; 
    add_action_record.out_formats[1].field_list[0].field_offset = 0;
    add_action_record.out_formats[1].field_list[1].field_name = "polygon_points"; 
    add_action_record.out_formats[1].field_list[1].field_type = "PipelinedPoint[num_points]"; 
    add_action_record.out_formats[1].field_list[1].field_size = 12; 
    add_action_record.out_formats[1].field_list[1].field_offset = 4;
    add_action_record.out_formats[1].field_list[2].field_name = ""; 
    add_action_record.out_formats[1].field_list[2].field_type = ""; 
    add_action_record.out_formats[1].field_list[2].field_size = 0; 
    add_action_record.out_formats[1].field_list[2].field_offset = 0;
    add_action_record.func_str = "{ la la la }";
}

void
free_written_data()
{
    int i, j, k, index;
    k = 0;
    for (i = 1; i < 20; i += 5) {
	for (j = 0; j < rec6_array[k].icount; j++) {
	    free(rec6_array[k].var_string_array[j].string);
	}
	free(rec6_array[k].string);
	free(rec6_array[k].var_int_array);
	free(rec6_array[k].var_double_array);
	free(rec6_array[k].var_string_array);
	k++;
    }
    index = 0;
    for (i = 1; i < 10; i += 2) {
	for (j = 0; j < rec9_array[index].vec_length; j++) {
	    free(rec9_array[index].eventv[j].iov_base);
	    free(string_array_array[index].array[j]);
	}
	free(rec9_array[index].eventv);
	if (string_array_array[index].base_string != NULL) 
	    free(string_array_array[index].base_string);
	free(string_array_array[index].array);
	index++;
    }
    free(multi_array.int_array);
    free(multi_array.int_array2);
    free(multi_array.int_array3);
    free(multi_array2.int_array);
    free(multi_array2.int_array2);
    free(multi_array2.int_array3);
    free(fortran_array.int_array);
    free(fortran_array.int_array2);
    free(fortran_array.int_array3);
    free(derive.client_channel_id.channel);
    free(derive.field_list);
    free(derive.format_list[0].field_list);
    free(derive.format_list[1].field_list);
    free(derive.format_list);
    for (i=0; i<triangle.num_meshes; i++) {
	free(triangle.mesh_data[i].compressed_data);
    }
    free(triangle.mesh_data);
    free(triangle.codebook_data);
    for (i=0; i<add_action_record.format_count; i++) {
	free(add_action_record.out_formats[i].field_list);
    }
    free(add_action_record.out_formats);
}

FMField field_list[] = {
    {"integer field(14)", "integer", 
       sizeof(int), FMOffset(first_rec_ptr, integer_field)},
    {"double field", "float",
       sizeof(double), FMOffset(first_rec_ptr, double_field)},
    {"char field(A)", "char",
       sizeof(char), FMOffset(first_rec_ptr, char_field)},
    { NULL, NULL, 0, 0}
};

FMField newer_field_list[] = {
    {"ganzzahlfeld", "integer", 
       sizeof(int), FMOffset(newer_rec_ptr, ganzzahl)},
    {"gleitkommazahlfeld", "float",
       sizeof(double), FMOffset(newer_rec_ptr, gleitkommazahl)},
    {"zeichenfeld", "char",
       sizeof(char), FMOffset(newer_rec_ptr, zeichen)},
    { NULL, NULL, 0, 0}
};

FMField field_list2[] = {
    {"integer field(-14)", "integer", 
       sizeof(int), FMOffset(second_rec_ptr, integer_field)},
    {"short field", "integer", 
       sizeof(short), FMOffset(second_rec_ptr, short_field)},
    {"long field(-9876129)", "integer", 
       sizeof(long), FMOffset(second_rec_ptr, long_field)},
    {"string field", "string",
       sizeof(char *), FMOffset(second_rec_ptr, string)},
    {"double field(2.717)", "float",
       sizeof(double), FMOffset(second_rec_ptr, double_field)},
    {"char field", "char",
       sizeof(char), FMOffset(second_rec_ptr, char_field)},
    { NULL, NULL, 0, 0}
};

FMField field_list3[] = {
    {"integer field", "integer", 
       sizeof(int), FMOffset(third_rec_ptr, integer_field)},
    {"long field", "integer", 
       sizeof(long), FMOffset(third_rec_ptr, long_field)},
    {"uint field(4159015374)", "unsigned integer", 
       sizeof(int), FMOffset(third_rec_ptr, uint_field)},
    {"ulong field(3969765680)", "unsigned integer", 
       sizeof(long), FMOffset(third_rec_ptr, ulong_field)},
    {"string field", "string",
       sizeof(char *), FMOffset(third_rec_ptr, string)},
    {"double field(2.717)", "float",
       sizeof(double), FMOffset(third_rec_ptr, double_field)},
    {"string field2", "string",
       sizeof(char *), FMOffset(third_rec_ptr, string2)},
    {"char field", "char",
       sizeof(char), FMOffset(third_rec_ptr, char_field)},
    {"enum field(1)", "enumeration",
       sizeof(enum_type), FMOffset(third_rec_ptr, enum_field)},
    { NULL, NULL, 0, 0}
};

FMField field_list4[] = {
    {"ifield", "integer", 
       sizeof(long), FMOffset(fourth_rec_ptr, ifield)},
    {"int_array", FMArrayDecl(integer,ARRAY_SIZE), 
       sizeof(int), FMOffset(fourth_rec_ptr, int_array[0])},
    {"double field", "float[2][2]",
       sizeof(double), FMOffset(fourth_rec_ptr, double_array[0][0])},
    { NULL, NULL, 0, 0}
};

FMField embedded_field_list[] = {
    {"ifield", "integer",
       sizeof(short), FMOffset(embedded_rec_ptr, ifield)},
    {"string field", "string",
       sizeof(char *), FMOffset(embedded_rec_ptr, string)},
    {"dfield", "float",
       sizeof(double), FMOffset(embedded_rec_ptr, dfield)},
    {NULL, NULL, 0, 0}
};

FMField field_list5[] = {
    {"earray", "embedded[4]",
       sizeof(embedded_rec), FMOffset(fifth_rec_ptr, earray)},
    {NULL, NULL, 0, 0}
};

FMField later_field_list[] = {
    {"integer field", "integer", 
       sizeof(((later_rec_ptr)0)->integer_field), FMOffset(later_rec_ptr, integer_field)},
    {"string field", "string",
       sizeof(char *), FMOffset(later_rec_ptr, string)},
    {"double field", "float",
       sizeof(double), FMOffset(later_rec_ptr, double_field)},
    { NULL, NULL, 0, 0}
};

FMField later_field_list2[] = {
    {"integer field", "integer", 
       sizeof(((later_rec2_ptr)0)->integer_field), 
       FMOffset(later_rec2_ptr, integer_field)},
    {"string field", "string",
       sizeof(char *), FMOffset(later_rec2_ptr, string)},
    {"double field", "float",
       sizeof(double), FMOffset(later_rec2_ptr, double_field)},
    { NULL, NULL, 0, 0}
};

FMField nested_field_list[] = {
    {"integer field", "integer", 
       sizeof(((nested_rec_ptr)0)->integer_field), 
       FMOffset(nested_rec_ptr, integer_field)},
    {"nested record", "string format",
       sizeof(second_rec), FMOffset(nested_rec_ptr, nested_rec)},
    {"string field", "string",
       sizeof(char *), FMOffset(nested_rec_ptr, string)},
    { NULL, NULL, 0, 0}
};

FMField field_list6[] = {
    {"string field", "string",
       sizeof(char *), FMOffset(sixth_rec_ptr, string)},
    {"icount", "integer", 
       sizeof(long), FMOffset(sixth_rec_ptr, icount)},
    {"var_int_array", "integer[icount]",
       sizeof(((sixth_rec_ptr)0)->var_int_array[0]), FMOffset(sixth_rec_ptr, var_int_array)},
    {"var_string_array", "string format[icount]",
       sizeof(second_rec), FMOffset(sixth_rec_ptr, var_string_array)},
    {"double field", "float",
       sizeof(double), FMOffset(sixth_rec_ptr, dfield)},
    {"var_double_array", "float[icount]",
       sizeof(double), FMOffset(sixth_rec_ptr, var_double_array)},
    { NULL, NULL, 0, 0}
};

FMField event_vec_elem_fields[] =
{
    {"elem", "char[len]", sizeof(char), FMOffset(EncodeVector,iov_base)},
    {"len", "integer", sizeof(((EncodeVector)0)[0].iov_len), 
     FMOffset(EncodeVector, iov_len)},
    {(char *) 0, (char *) 0, 0, 0}
};

FMField field_list9[] =
{
    {"vec_length", "integer", sizeof(int),
     FMOffset(ninth_rec_ptr, vec_length)},
    {"eventv", "EventVecElem[vec_length]", sizeof(struct _io_encode_vec), 
     FMOffset(ninth_rec_ptr, eventv)},
    {(char *) 0, (char *) 0, 0, 0}
};

FMField string_array_field_list[] =
{
    {"array_len", "integer", sizeof(int),
     FMOffset(string_array_rec_ptr, array_len)},
    {"base_string", "string", sizeof(char*),
     FMOffset(string_array_rec_ptr, base_string)},
    {"array", "string[array_len]", sizeof(char*), 
     FMOffset(string_array_rec_ptr, array)},
    {(char *) 0, (char *) 0, 0, 0}

};

FMField channel_id_flds[] = {
    {"len", "integer", sizeof(int), FMOffset(channel_ID_struct *, len)},
    {"id", "char[len]", 1, FMOffset(channel_ID_struct *, channel)},
    {(char *) 0, (char *) 0, 0, 0}
};

FMField field_list_flds[] = {
    {"field_name", "string", sizeof(char *),
     FMOffset(FMFieldList, field_name)},
    {"field_type", "string", sizeof(char *),
     FMOffset(FMFieldList, field_type)},
    {"field_size", "integer", sizeof(int),
     FMOffset(FMFieldList, field_size)},
    {"field_offset", "integer", sizeof(int),
     FMOffset(FMFieldList, field_offset)},
    {(char *) 0, (char *) 0, 0, 0}
};

FMField format_list_field_list[] = {
    {"format_name", "string", sizeof(char *),
     FMOffset(format_list_element *, format_name)},
    {"field_list_len", "integer", sizeof(int),
     FMOffset(format_list_element *, field_list_len)},
    {"field_list", "IOfield_list[field_list_len]", sizeof(FMField),
     FMOffset(format_list_element *, field_list)}
    ,
    {(char *) 0, (char *) 0, 0, 0}
};

FMField derive_msg_field_list[] = {
    {"channel", "string", sizeof(char *),
     FMOffset(DeriveMsgPtr, chan_str)},
    {"condition", "integer", sizeof(int), FMOffset(DeriveMsgPtr, cond)},
    {"client_channel_id", "channel_ID", sizeof(channel_ID_struct),
     FMOffset(DeriveMsgPtr, client_channel_id)}
    ,
    {"client_contact_str", "string", sizeof(char *),
     FMOffset(DeriveMsgPtr, client_contact_str)},
    {"filter", "string", sizeof(char *), FMOffset(DeriveMsgPtr, filter)},
    {"field_list_len", "integer", sizeof(int),
     FMOffset(DeriveMsgPtr, field_list_len)},
    {"field_list", "IOfield_list[field_list_len]", sizeof(FMField),
     FMOffset(DeriveMsgPtr, field_list)}
    ,
    {"format_list_len", "integer", sizeof(int),
     FMOffset(DeriveMsgPtr, format_list_len)},
    {"format_list", "DEFormatList[format_list_len]",
     sizeof(format_list_element), FMOffset(DeriveMsgPtr, format_list)}
    ,
    {"init_data_block", "char[init_data_len]",
     1, FMOffset(DeriveMsgPtr, init_data_block)}
    ,
    {"init_data_len", "integer",
     sizeof(int), FMOffset(DeriveMsgPtr, init_data_len)},
    {(char *) 0, (char *) 0, 0, 0}
};

FMField multi_array_flds[] = {
    {"ifield", "integer", sizeof(long), FMOffset(multi_array_rec_ptr, ifield)},
    {"double_array", "float[2][2][2][2]", sizeof(double),
     FMOffset(multi_array_rec_ptr, double_array)},
    {"int_array", "integer[2][ifield]", sizeof(int),
     FMOffset(multi_array_rec_ptr, int_array)},
    {"int_array2", "integer[ifield][2]", sizeof(int),
     FMOffset(multi_array_rec_ptr, int_array2)},
    {"int_array3", "integer[ifield][ifield][ifield]", sizeof(int),
    FMOffset(multi_array_rec_ptr, int_array3)},
    {(char *) 0, (char *) 0, 0, 0}
};

FMField compressed_mesh[] = {
  {"id", "integer", sizeof(int), FMOffset(compressed_mesh_param_ptr,id)},
  {"data_size", "integer", sizeof(int), FMOffset(compressed_mesh_param_ptr,data_size)},
  {"old_data_size", "integer", sizeof(int), FMOffset(compressed_mesh_param_ptr,old_data_size)},
  {"compressed_size", "integer", sizeof(int), FMOffset(compressed_mesh_param_ptr,compressed_size)},
  {"compressed_data", "integer[compressed_size]", sizeof(int), FMOffset(compressed_mesh_param_ptr,compressed_data)},
  {"atom_type", "integer", sizeof(int), FMOffset(compressed_mesh_param_ptr,atom_type)},
  {"corner1x", "integer", sizeof(int), FMOffset(compressed_mesh_param_ptr,corner1x)},
  {"corner1y", "integer", sizeof(int), FMOffset(compressed_mesh_param_ptr,corner1y)},
  {"corner1z", "integer", sizeof(int), FMOffset(compressed_mesh_param_ptr,corner1z)},
  {"corner2x", "integer", sizeof(int), FMOffset(compressed_mesh_param_ptr,corner2x)},
  {"corner2y", "integer", sizeof(int), FMOffset(compressed_mesh_param_ptr,corner2y)},
  {"corner2z", "integer", sizeof(int), FMOffset(compressed_mesh_param_ptr,corner2z)},
  {NULL, NULL, 0 , 0}
};

FMField triangle_field[] = {
  {"timestamp", "integer", sizeof(int), FMOffset(triangle_param_ptr,timestamp)},
  {"nonce", "integer", sizeof(int), FMOffset(triangle_param_ptr, nonce)},
  //  {"num_ints", "integer", sizeof(int), FMOffset(triangle_param_ptr,num_ints)},
  //  {"triangle_data", "integer[num_ints]", sizeof(int), FMOffset(triangle_param_ptr,triangle_data)},
  {"num_meshes", "integer", sizeof(int), FMOffset(triangle_param_ptr,num_meshes)},
  {"mesh_data", "compressed_mesh_param[num_meshes]", sizeof(compressed_mesh_param), FMOffset(triangle_param_ptr,mesh_data)},

  {"compression_type", "integer", sizeof(int), FMOffset(triangle_param_ptr, compression_type)},

  {"codebook_size", "integer", sizeof(int), FMOffset(triangle_param_ptr,codebook_size)},
  {"codebook_data", "integer[codebook_size]", sizeof(int), FMOffset(triangle_param_ptr,codebook_data)},

  {"corner1x", "integer", sizeof(int), FMOffset(triangle_param_ptr, corner1x)},
  {"corner1y", "integer", sizeof(int), FMOffset(triangle_param_ptr, corner1y)},
  {"corner1z", "integer", sizeof(int), FMOffset(triangle_param_ptr, corner1z)},
  {"corner2x", "integer", sizeof(int), FMOffset(triangle_param_ptr, corner2x)},
  {"corner2y", "integer", sizeof(int), FMOffset(triangle_param_ptr, corner2y)},
  {"corner2z", "integer", sizeof(int), FMOffset(triangle_param_ptr, corner2z)},

  {NULL, NULL, 0 , 0}
};
    
FMField add_field_list[] =
{
    {"action_type", "integer",
     sizeof(int), FMOffset(add_rec_ptr, action)},
    {"in_format_name", "string",
     sizeof(char*), FMOffset(add_rec_ptr, in_format_name)},
    {"format_count", "integer",
     sizeof(int), FMOffset(add_rec_ptr, format_count)},
    {"out_formats", "XMLFormatList[format_count]",
     sizeof(msg_format_list_element), FMOffset(add_rec_ptr, out_formats)},
    {"func_str", "string",
     sizeof(char*), FMOffset(add_rec_ptr, func_str)},
    {NULL, NULL, 0, 0}
};

FMField xml_format_list_flds[] =
{
    {"format_name", "string", sizeof(char *), 
     FMOffset(msg_format_list_element*, format_name)},
    {"field_list_len", "integer", sizeof(int), 
     FMOffset(msg_format_list_element*, field_list_len)},
    {"field_list", "IOfield_list[field_list_len]", sizeof(FMField),
     FMOffset(msg_format_list_element*, field_list)},
    {"xml_markup", "string",
     sizeof(char*), FMOffset(msg_format_list_element*, xml_markup)},
    {(char *) 0, (char *) 0, 0, 0}
};

