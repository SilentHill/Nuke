
#include "DataTable.h"
#include "_pch.h"

namespace Nuke::System::Data
{

    class DataTable::DataTableImpl
    {
    public:

        std::vector<std::unordered_map<std::string, std::string>> table;
    };

    DataTable::DataTable()
    {
        impl = std::make_unique<DataTableImpl>();
    }

    DataTable::~DataTable()
    {

    }
}