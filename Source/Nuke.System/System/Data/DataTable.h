
#pragma once

namespace Nuke::System::Data
{
	class DataTable final
	{
	public:
		DataTable();
		~DataTable();
		DataTable(const DataTable&);
		DataTable(DataTable&&) = delete;

		std::vector<std::string> GetColumns();

	private:
		class DataTableImpl;
		std::unique_ptr<DataTableImpl> impl;
	};
}