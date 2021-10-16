#pragma once

namespace Memory
{
	class Module : public Range
	{
	public:
		Module(HMODULE mod);
		explicit Module(std::nullptr_t);
		explicit Module(std::string_view name);
		explicit Module(std::wstring_view name);

	    Handle GetExport(std::string_view symbol_name);
	};
}
