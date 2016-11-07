#ifndef _JSON_DOCUMENT_H_
#define _JSON_DOCUMENT_H_

struct json_object_t;
typedef struct json_object_t JSON_Object;

struct json_value_t;
typedef struct json_value_t JSON_Value;


class JSon_Document
{
public:

	//Constructors
	JSon_Document();
	JSon_Document(const char* file_name);
	JSon_Document(JSON_Object* node);

	//Destructor
	~JSon_Document();


	//Functions
	JSon_Document Get_Node(const char* name_node)const;
	const char* Get_String(const char* name_node)const;

private:

	JSON_Value* r_node_value = nullptr;
	JSON_Object* r_node = nullptr;
	


};

#endif
