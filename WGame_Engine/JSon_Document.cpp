#include "JSon_Document.h"
#include "parson.h"


//Constructors
JSon_Document::JSon_Document()
{
	r_node_value = json_value_init_object();
	r_node = json_value_get_object(r_node_value);
}

JSon_Document::JSon_Document(const char* file_name)
{
	r_node_value = json_parse_string(file_name);
	r_node = json_value_get_object(r_node_value);
}

JSon_Document::JSon_Document(JSON_Object* node)
{
	r_node = node;
}

//Destructor
JSon_Document::~JSon_Document()
{
	json_value_free(r_node_value);
}

//Functions
JSon_Document JSon_Document::Get_Node(const char* name_node)const
{
	return JSon_Document(json_object_get_object(r_node, name_node));
}

const char* JSon_Document::Get_String(const char* name_node)const
{
	return json_object_get_string(r_node, name_node);
}