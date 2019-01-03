#include "new_delete.h"

class Free_Delete
{
public:
	Free_Delete()
	{

	}
	~Free_Delete() = default;

	//wchar for UTF-16 path support
	static bool Delete_Model(wchar_t *Path);
	static bool Free_Handle(wchar_t *Path);
};