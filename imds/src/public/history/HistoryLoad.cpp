/*
 * HistoryLoad.cpp
 *
 *  Created on: 2016/10/14/
 *      Author: user
 */

#include "HistoryLoad.h"
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include "../../util/atof/Parse.h"

HistoryLoad::HistoryLoad(CShmEnv* shmEnv, char* shmname, char* hispath)
	: m_shmEnv(shmEnv),
	  m_shmName(shmname),
	  m_hisPath(hispath) {
	// TODO Auto-generated constructor stub

}

HistoryLoad::~HistoryLoad() {
	// TODO Auto-generated destructor stub
}

/*
void HistoryLoad::load(const char* filename) {
	assert(m_shmEnv != NULL);

	//read the file
	fstream infile;
	infile.open(filename, ios::in);
	if (!infile.is_open()) {
		//ERROR
		return;
	}

	string data;
	while (!infile.eof()) {
		getline(infile, data, '\n');
		if (!data.empty())
			Convert2BinaryDay(data);
	}
	infile.close();

	for (SHM_CANDLESTICK_MAP::const_iterator cit = m_mDay.begin(); cit != m_mDay.end(); ++cit) {
		//obtain shm object
		string shmField = m_shmName + "/" + cit->first + ".day";
		CShmVector* shmVec = m_shmEnv->getShmVector(shmField.c_str(), sizeof(CANDLESTICK));
		if (shmVec == NULL) {
			//ERROR("Get shm vector failed!");
			return;
		}

		shmVec->push_back((char*)cit->second.get());
	}
}*/

void HistoryLoad::load(uint32_t date) {
	assert(m_shmEnv != NULL);

	ostringstream oss;
	oss << m_hisPath << "/" << date << "/Day.csv";

	//read the file
	fstream infile;
	infile.open(oss.str().c_str(), ios::in);
	if (!infile.is_open()) {
		//ERROR
		return;
	}

	string data;
	while (!infile.eof()) {
		getline(infile, data, '\n');
		if (!data.empty())
			Convert2BinaryDay(data);
	}
	infile.close();

	for (SHM_CANDLESTICK_MAP::const_iterator cit = m_mDay.begin(); cit != m_mDay.end(); ++cit) {
		//obtain shm object
		string shmField = m_shmName + "/" + cit->first + ".day";
		CShmVector* shmVec = m_shmEnv->getShmVector(shmField.c_str(), sizeof(CANDLESTICK), 256);
		if (shmVec == NULL) {
			//ERROR("Get shm vector failed!");
			return;
		}

		if (shmVec->empty()) {
			//push back
			shmVec->push_back((char*)cit->second.get());
			continue;
		}

		int vecSize = shmVec->size();
		bool isBack = true;
		bool isFront = true;
		//iterate shmVec from back
		for (int i = vecSize - 1; i >= 0; --i) {
			CANDLESTICK dayk = *(CANDLESTICK*)(shmVec->at(i));
			uint32_t shmDate = dayk.dateTime;
			if (shmDate > date) {
				isBack = false;
				continue;
			}
			else if (shmDate == date) {
				//override
				memcpy(shmVec->at(i), cit->second.get(), sizeof(CANDLESTICK));
				isFront = false;
				break;
			}
			else {
				if (isBack) {
					//push back
					shmVec->push_back((char*)cit->second.get());
				}
				else {
					shmVec->push_back(shmVec->at(vecSize - 1));
					for (int j = vecSize - 1; j > i + 1; --j) {
						memcpy(shmVec->at(j), shmVec->at(j - 1), sizeof(CANDLESTICK));
					}
					memcpy(shmVec->at(i + 1), cit->second.get(), sizeof(CANDLESTICK));
				}
				isFront = false;
				break;
			}
		}//for
		if (isFront) {
			//move back
			shmVec->push_back(shmVec->at(vecSize - 1));
			for (int i = vecSize - 1; i > 0; --i) {
				memcpy(shmVec->at(i), shmVec->at(i - 1), sizeof(CANDLESTICK));
			}
			memcpy(shmVec->at(0), cit->second.get(), sizeof(CANDLESTICK));
		}
	}
}

void HistoryLoad::loadAll() {
	assert(m_shmEnv != NULL);

	DIR *pdir = NULL;
	struct dirent *pdirent = NULL;
	if ((pdir = opendir(m_hisPath.c_str())) == NULL) {
		//ERROR("opendir: %s failed!", path);
		return;
	}

	vector<string> vtDates;
	while ((pdirent = readdir(pdir)) != NULL) {
		if (strcmp(pdirent->d_name, ".") != 0 && strcmp(pdirent->d_name, "..") != 0) {
			vtDates.push_back(pdirent->d_name);
		}
	}
	closedir(pdir);
	//load all dayk data
	sort(vtDates.begin(), vtDates.end());

	vector<string>::iterator itDate = vtDates.begin();
	for (; itDate != vtDates.end(); ++itDate) {
		string filename;
		filename = m_hisPath + "/" + *itDate + "/Day.csv";

		//read the file
		fstream infile;
		infile.open(filename.c_str(), ios::in);
		if (!infile.is_open()) {
			//ERROR
			return;
		}

		string data;
		while (!infile.eof()) {
			getline(infile, data, '\n');
			if (!data.empty())
				Convert2BinaryDay(data);
		}
		infile.close();

		for (SHM_CANDLESTICK_MAP::const_iterator cit = m_mDay.begin(); cit != m_mDay.end(); ++cit) {
			//obtain shm object
			string shmField = m_shmName + "/" + cit->first + ".day";
			CShmVector* shmVec = m_shmEnv->getShmVector(shmField.c_str(), sizeof(CANDLESTICK), 256);
			if (shmVec == NULL) {
				//ERROR("Get shm vector failed!");
				return;
			}

			shmVec->push_back((char*)cit->second.get());
		}
	}
}

/*
void HistoryLoad::load(const char* filename, uint32_t limit) {
	assert(m_shmEnv != NULL);

	//read the file
	fstream infile;
	infile.open(filename, ios::in);
	if (!infile.is_open()) {
		//ERROR
		return;
	}

	string data;
	while (!infile.eof()) {
		getline(infile, data, '\n');
		if (!data.empty())
			Convert2BinaryMinute(data, limit);
	}
	infile.close();

	for (SHM_CANDLESTICK_MAP_MAP::const_iterator cit = m_mmMinute.begin();
			cit != m_mmMinute.end(); ++cit) {
		//obtain shm object
		string shmField = m_shmName + "/" + cit->first + ".min";
		CShmVector* shmVec = m_shmEnv->getShmVector(shmField.c_str(), sizeof(CANDLESTICK));
		if (shmVec == NULL) {
			//ERROR("Get shm vector failed!");
			return;
		}

		int num = cit->second.size();
		int diff = shmVec->size() + num - limit;
		if (diff > 0) {
			//reach the limit
			//move data
			for (int i = num; i < shmVec->size(); ++i) {
				memcpy(shmVec->at(i - num), shmVec->at(i), sizeof(CANDLESTICK));
			}
			//insert the new data
			int j = 0;
			for (map<uint64_t, boost::shared_ptr<CANDLESTICK> >::const_iterator dtit = cit->second.begin();
					dtit != cit->second.end(); ++dtit, ++j) {
				memcpy(shmVec->at(shmVec->size() - num + j), dtit->second.get(), sizeof(CANDLESTICK));
			}
		}
		else {
			for (map<uint64_t, boost::shared_ptr<CANDLESTICK> >::const_iterator dtit = cit->second.begin();
					dtit != cit->second.end(); ++dtit) {
				shmVec->push_back((char*)dtit->second.get());
			}
		}

	}
}*/

void HistoryLoad::load(uint32_t date, uint32_t limit) {
	assert(m_shmEnv != NULL);

	ostringstream oss;
	oss << m_hisPath << "/" << date << "/Minute.csv";

	//read the file
	fstream infile;
	infile.open(oss.str().c_str(), ios::in);
	if (!infile.is_open()) {
		//ERROR
		return;
	}

	string data;
	while (!infile.eof()) {
		getline(infile, data, '\n');
		if (!data.empty())
			Convert2BinaryMinute(data, limit);
	}
	infile.close();

	for (SHM_CANDLESTICK_MAP_MAP::const_iterator cit = m_mmMinute.begin();
			cit != m_mmMinute.end(); ++cit) {
		//obtain shm object
		string shmField = m_shmName + "/" + cit->first + ".min";
		CShmVector* shmVec = m_shmEnv->getShmVector(shmField.c_str(), sizeof(CANDLESTICK), limit);
		if (shmVec == NULL) {
			//ERROR("Get shm vector failed!");
			return;
		}

		if (shmVec->empty()) {
			for (map<uint64_t, boost::shared_ptr<CANDLESTICK> >::const_iterator dtit = cit->second.begin();
					dtit != cit->second.end(); ++dtit)
				shmVec->push_back((char*)dtit->second.get());
			continue;
		}

		int vecSize = shmVec->size();
		int num = cit->second.size();
		bool isFront = true;
		bool isBack = true;
		//iterate shmVec from back
		for (int i = vecSize - num; i >= 0; i -= num) {
			CANDLESTICK mink = *(CANDLESTICK*)(shmVec->at(i));
			uint32_t shmDate = mink.dateTime / 1000000;
			if (shmDate > date) {
				//iterate forward
				isBack = false;
				continue;
			}
			else if (shmDate == date) {
				//override
				int j = 0;
				for (map<uint64_t, boost::shared_ptr<CANDLESTICK> >::const_iterator dtit = cit->second.begin();
					dtit != cit->second.end(); ++dtit, ++j) {
					memcpy(shmVec->at(i + j), dtit->second.get(), sizeof(CANDLESTICK));
				}
				isFront = false;
				break;
			}
			else {
				if (vecSize + num > limit) {
					//move front ones and insert
					for (int j = 0; j < i; ++j) {
						memcpy(shmVec->at(j), shmVec->at(j + num),	sizeof(CANDLESTICK));
					}
					int k = 0;
					for (map<uint64_t, boost::shared_ptr<CANDLESTICK> >::const_iterator dtit =
							cit->second.begin(); dtit != cit->second.end();	++dtit, ++k) {
						memcpy(shmVec->at(i + k), dtit->second.get(), sizeof(CANDLESTICK));
					}
				}
				else if(!isBack) {
					//move back ones
					for (int j = vecSize - num; j < vecSize; ++j) {
						shmVec->push_back(shmVec->at(j));
					}
					for (int j = vecSize - num - 1; j >= i + num; --j) {
						memcpy(shmVec->at(j + num), shmVec->at(j), sizeof(CANDLESTICK));
					}
					//override
					int k = 0;
					for (map<uint64_t, boost::shared_ptr<CANDLESTICK> >::const_iterator dtit =
							cit->second.begin(); dtit != cit->second.end(); ++dtit, ++k) {
							memcpy(shmVec->at(i + num + k), dtit->second.get(), sizeof(CANDLESTICK));
					}
				}
				else {
					//push back
					for (map<uint64_t, boost::shared_ptr<CANDLESTICK> >::const_iterator dtit =
						cit->second.begin(); dtit != cit->second.end();	++dtit) {
						shmVec->push_back((char*)dtit->second.get());
					}
				}

				isFront = false;
				break;
			}
		}//for i

		if (isFront && vecSize + num <= limit) {
			//move back ones
			for (int j = vecSize - num; j < vecSize; ++j) {
				shmVec->push_back(shmVec->at(j));
			}
			for (int j = vecSize - num - 1; j >= 0; --j) {
				memcpy(shmVec->at(j + num), shmVec->at(j), sizeof(CANDLESTICK));
			}
			//override
			int k = 0;
			for (map<uint64_t, boost::shared_ptr<CANDLESTICK> >::const_iterator dtit =
					cit->second.begin(); dtit != cit->second.end(); ++dtit, ++k) {
					memcpy(shmVec->at(k), dtit->second.get(), sizeof(CANDLESTICK));
			}
		}

	}//for cit
}

void HistoryLoad::loadAll(uint32_t limit) {
	assert(m_shmEnv != NULL);

	DIR *pdir = NULL;
	struct dirent *pdirent = NULL;
	if ((pdir = opendir(m_hisPath.c_str())) == NULL) {
		//ERROR("opendir: %s failed!", path);
		return;
	}

	vector<string> vtDates;
	while ((pdirent = readdir(pdir)) != NULL) {
		if (strcmp(pdirent->d_name, ".") != 0 && strcmp(pdirent->d_name, "..") != 0) {
			vtDates.push_back(pdirent->d_name);
		}
	}
	closedir(pdir);
	//load latest data
	sort(vtDates.rbegin(), vtDates.rend());

	vector<string>::iterator itDate = vtDates.begin();
	for (; itDate != vtDates.end(); ++itDate) {
		string filename;
		filename = m_hisPath + "/" + *itDate + "/Minute.csv";

		//read the file
		fstream infile;
		infile.open(filename.c_str(), ios::in);
		if (!infile.is_open()) {
			//ERROR
			return;
		}

		string data;
		while (!infile.eof()) {
			getline(infile, data, '\n');
			if (!data.empty())
				Convert2BinaryMinute(data, limit);
		}
		infile.close();
	}


	for (SHM_CANDLESTICK_MAP_MAP::const_iterator cit = m_mmMinute.begin();
			cit != m_mmMinute.end(); ++cit) {
		//obtain shm object
		string shmField = m_shmName + "/" + cit->first + ".min";
		CShmVector* shmVec = m_shmEnv->getShmVector(shmField.c_str(), sizeof(CANDLESTICK), limit);
		if (shmVec == NULL) {
			//ERROR("Get shm vector failed!");
			return;
		}

		for (map<uint64_t, boost::shared_ptr<CANDLESTICK> >::const_iterator dtit = cit->second.begin();
				dtit != cit->second.end(); ++dtit) {
			shmVec->push_back((char*)dtit->second.get());
		}
	}
}

void HistoryLoad::Clear() {
	int count = 0;
	SHM_OBJ_INFO_LIST objInfoList;
	if (m_shmEnv->getShmObjInfo(objInfoList)) {
		for (unsigned int i = 0; i < objInfoList.size(); ++i) {
			if (objInfoList[i].shmObjName.find(m_shmName) != string::npos &&
					( objInfoList[i].shmObjName.find(".day") != string::npos ||
					objInfoList[i].shmObjName.find(".min") != string::npos) ) {
				CShmVector* shmVec = m_shmEnv->getShmVector(objInfoList[i].shmObjName.c_str());
				if (shmVec != NULL) {
					shmVec->clear();
					++count;
				}
			}
		}
	}
	//INFO("clear %s objects (%d)", m_shmName, count);
}

void HistoryLoad::Convert2BinaryDay(string& line) {
	Parse ps;
	size_t pos = line.find(",");
	string code = line.substr(0, pos);  //code
	line = line.substr(pos + 1);

	pos = line.find(",");
	if (m_mDay.find(code) == m_mDay.end())
		m_mDay[code] = boost::make_shared<CANDLESTICK>();
	m_mDay[code]->dateTime = atol(line.substr(0, pos).c_str());  //datetime
	line = line.substr(pos + 1);

	if (m_shmName == "sho") {
		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //preClosePx
		m_mDay[code]->prevClosePx = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
		line = line.substr(pos + 1);

		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //OpenPx
		m_mDay[code]->openPx = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
		line = line.substr(pos + 1);

		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //HighPx
		m_mDay[code]->highPx = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
		line = line.substr(pos + 1);

		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //LowPx
		m_mDay[code]->lowPx = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
		line = line.substr(pos + 1);

		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //LastPx
		m_mDay[code]->lastPx = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
		line = line.substr(pos + 1);

		pos = line.find(",");
		m_mDay[code]->volume = atol(line.substr(0, pos).c_str());  //Volume
		line = line.substr(pos + 1);

		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //Amount
		m_mDay[code]->amount = ps.getMantissa() * pow(10.0L, 2 + ps.getExponent());
		line = line.substr(pos + 1);

		m_mDay[code]->openInterest = atol(line.c_str());  //OpenInterest
		m_mDay[code]->iopv = 0;
	}
	else {
		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //preClosePx
		m_mDay[code]->prevClosePx = ps.getMantissa() * pow(10.0L, 3 + ps.getExponent());
		line = line.substr(pos + 1);

		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //OpenPx
		m_mDay[code]->openPx = ps.getMantissa() * pow(10.0L, 3 + ps.getExponent());
		line = line.substr(pos + 1);

		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //HighPx
		m_mDay[code]->highPx = ps.getMantissa() * pow(10.0L, 3 + ps.getExponent());
		line = line.substr(pos + 1);

		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //LowPx
		m_mDay[code]->lowPx = ps.getMantissa() * pow(10.0L, 3 + ps.getExponent());
		line = line.substr(pos + 1);

		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //LastPx
		m_mDay[code]->lastPx = ps.getMantissa() * pow(10.0L, 3 + ps.getExponent());
		line = line.substr(pos + 1);

		pos = line.find(",");
		m_mDay[code]->volume = atol(line.substr(0, pos).c_str()) * 100;  //Volume
		line = line.substr(pos + 1);

		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //Amount
		m_mDay[code]->amount = ps.getMantissa() * pow(10.0L, ps.getExponent());
		line = line.substr(pos + 1);

		ps.parse(line);
		m_mDay[code]->iopv = ps.getMantissa() * pow(10.0L, 3 + ps.getExponent());
		m_mDay[code]->openInterest = 0;
	}
	m_mDay[code]->avgPx = m_mDay[code]->yield2Mature = m_mDay[code]->finance
			= m_mDay[code]->securityLending = m_mDay[code]->inValue = m_mDay[code]->timeValue = 0;
}

void HistoryLoad::Convert2BinaryMinute(string& line, uint32_t limit) {
	Parse ps;
	size_t pos = line.find(",");
	string code = line.substr(0, pos);  //code
	if (m_mmMinute[code].size() + 1 > limit)
		return;
	line = line.substr(pos + 1);

	pos = line.find(",");
	uint64_t datetime = atol(line.substr(0, pos).c_str());  //datetime
	m_mmMinute[code][datetime] = boost::make_shared<CANDLESTICK>();
	m_mmMinute[code][datetime]->dateTime = datetime;
	line = line.substr(pos + 1);

	if (m_shmName == "sho") {
		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //preClosePx
		m_mmMinute[code][datetime]->prevClosePx = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
		line = line.substr(pos + 1);

		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //OpenPx
		m_mmMinute[code][datetime]->openPx = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
		line = line.substr(pos + 1);

		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //HighPx
		m_mmMinute[code][datetime]->highPx = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
		line = line.substr(pos + 1);

		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //LowPx
		m_mmMinute[code][datetime]->lowPx = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
		line = line.substr(pos + 1);

		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //LastPx
		m_mmMinute[code][datetime]->lastPx = ps.getMantissa() * pow(10.0L, 4 + ps.getExponent());
		line = line.substr(pos + 1);

		pos = line.find(",");
		m_mmMinute[code][datetime]->volume = atol(line.substr(0, pos).c_str());  //Volume
		line = line.substr(pos + 1);

		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //Amount
		m_mmMinute[code][datetime]->amount = ps.getMantissa() * pow(10.0L, 2 + ps.getExponent());
		line = line.substr(pos + 1);

		m_mmMinute[code][datetime]->openInterest = atol(line.c_str());  //OpenInterest
		m_mmMinute[code][datetime]->iopv = 0;
	}
	else {
		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //preClosePx
		m_mmMinute[code][datetime]->prevClosePx = ps.getMantissa() * pow(10.0L, 3 + ps.getExponent());
		line = line.substr(pos + 1);

		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //OpenPx
		m_mmMinute[code][datetime]->openPx = ps.getMantissa() * pow(10.0L, 3 + ps.getExponent());
		line = line.substr(pos + 1);

		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //HighPx
		m_mmMinute[code][datetime]->highPx = ps.getMantissa() * pow(10.0L, 3 + ps.getExponent());
		line = line.substr(pos + 1);

		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //LowPx
		m_mmMinute[code][datetime]->lowPx = ps.getMantissa() * pow(10.0L, 3 + ps.getExponent());
		line = line.substr(pos + 1);

		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //LastPx
		m_mmMinute[code][datetime]->lastPx = ps.getMantissa() * pow(10.0L, 3 + ps.getExponent());
		line = line.substr(pos + 1);

		pos = line.find(",");
		m_mmMinute[code][datetime]->volume = atol(line.substr(0, pos).c_str()) * 100;  //Volume
		line = line.substr(pos + 1);

		pos = line.find(",");
		ps.parse(line.substr(0, pos));  //Amount
		m_mmMinute[code][datetime]->amount = ps.getMantissa() * pow(10.0L, ps.getExponent());
		line = line.substr(pos + 1);

		ps.parse(line);
		m_mmMinute[code][datetime]->iopv = ps.getMantissa() * pow(10.0L, 3 + ps.getExponent());
		m_mmMinute[code][datetime]->openInterest = 0;
	}
	m_mmMinute[code][datetime]->avgPx = m_mmMinute[code][datetime]->yield2Mature
			= m_mmMinute[code][datetime]->finance = m_mmMinute[code][datetime]->securityLending
			= m_mmMinute[code][datetime]->inValue = m_mmMinute[code][datetime]->timeValue = 0;

}

