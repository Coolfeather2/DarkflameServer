#include "CDProximityMonitorComponentTable.h"

void CDProximityMonitorComponentTable::LoadValuesFromDatabase() {

	// First, get the size of the table
	unsigned int size = 0;
	auto tableSize = CDClientDatabase::ExecuteQuery("SELECT COUNT(*) FROM ProximityMonitorComponent");
	while (!tableSize.eof()) {
		size = tableSize.getIntField(0, 0);

		tableSize.nextRow();
	}

	tableSize.finalize();

	// Reserve the size
	this->entries.reserve(size);

	// Now get the data
	auto tableData = CDClientDatabase::ExecuteQuery("SELECT * FROM ProximityMonitorComponent");
	while (!tableData.eof()) {
		CDProximityMonitorComponent entry;
		entry.id = tableData.getIntField("id", -1);
		entry.Proximities = tableData.getStringField("Proximities", "");
		entry.LoadOnClient = tableData.getIntField("LoadOnClient", -1);
		entry.LoadOnServer = tableData.getIntField("LoadOnServer", -1);

		this->entries.push_back(entry);
		tableData.nextRow();
	}

	tableData.finalize();
}

std::vector<CDProximityMonitorComponent> CDProximityMonitorComponentTable::Query(std::function<bool(CDProximityMonitorComponent)> predicate) {

	std::vector<CDProximityMonitorComponent> data = cpplinq::from(this->entries)
		>> cpplinq::where(predicate)
		>> cpplinq::to_vector();

	return data;
}

const std::vector<CDProximityMonitorComponent>& CDProximityMonitorComponentTable::GetEntries() const {
	return this->entries;
}

