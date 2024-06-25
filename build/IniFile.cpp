#include "IniFile.h"

std::string ConvertToLowercase(const std::string& str) { 
    std::string result = ""; 
    for (char ch : str) {  
        result += tolower(ch); 
    } 
    return result; 
} 

size_t CharCount(const std::string s, char c) {
    size_t count = 0, size = s.size();
    for (size_t i=0; i<size; i++) {
        if (s[i] == c) count++;
    }
    return count;
}

void ParseFromEnding(std::string& s, char c) {
    while (s.size() != 0) {
        if (s[s.size()-1] == c) s.erase(s.size()-1, 1);
        else break;
    }
}


void ParseFromBeginning(std::string& s, char c) {
    while (s.size() != 0) {
        if (s[0] == c) s.erase(0, 1);
        else break;
    }

}

void ParseFromSpaces(std::string& s) {
    //chars < 32 are invisible or special. char 32 = space
    while (s.size() != 0) {
        if (s[0] <= 32) s.erase(0, 1);
        else break;
    }
    while (s.size() != 0) {
        if (s[s.size()-1] <= 32) s.erase(s.size()-1, 1);
        else break;
    }
}

std::string ParseFromSpacesReturn(const std::string& s_in) {
    //chars < 32 are invisible or special. char 32 = space
    std::string s(s_in); 
    while (s.size() != 0) {
        if (s[0] <= 32) s.erase(0, 1);
        else break;
    }
    while (s.size() != 0) {
        if (s[s.size()-1] <= 32) s.erase(s.size()-1, 1);
        else break;
    }
    return s;
}


IniFile::IniFile(const std::string& path): filePath(ParseFromSpacesReturn(path)) {
    if (filePath == "") throw std::invalid_argument("File path cannot be empty");

    std::ifstream infile(filePath, std::ios::in);
    std::string line, section="", key, value;
    size_t pos;
    KeysMap KM;
    while (std::getline(infile, line)) {
        pos = line.find(";");
        if (pos != std::string::npos)
            line.resize(pos);

        if (line.empty()) 
            continue;

        ParseFromSpaces(line);

        if (line.empty()) 
            continue;

        if (line.find('[') != std::string::npos || line.find(']') != std::string::npos) {
            //Section line
            ParseFromBeginning(line, '['); ParseFromEnding(line, ']');
            if (line.find('[') != std::string::npos || line.find(']') != std::string::npos) continue;

            ParseFromSpaces(line);
            if (line.empty()) continue;

            if (!section.empty()) {
                if (data.count(section) != 0) 
                    {KM.insert(data[section].begin(), data[section].end());
                    data[section] = KM;}
                else
                    data.insert({section, std::move(KM)});
                KM.clear();
            }
            section = line;
        } else if (CharCount(line, '=') == 1) {
            //Key line
            pos = line.find('=');
            value = line.substr(pos+1);
            line.resize(pos); key = line;
            ParseFromSpaces(key); ParseFromSpaces(value);
            if (!key.empty() && !value.empty()) 
                KM[key] = value;
        }
    }
    //last section appending
    if (!section.empty()) {
        if (data.count(section) != 0) 
            {KM.insert(data[section].begin(), data[section].end());
            data[section] = KM;}
        else
            data.insert({section, std::move(KM)});
    }

    infile.close();
}


IniFile::~IniFile() {
    save();
}

void IniFile::save() {
    std::ofstream outfile(filePath, std::ios::out);
    std::string segment, key, value;
    KeysMap KM;
    for (auto segit = data.begin(); segit != data.end(); segit++) {
        segment = segit->first;
        KM = segit->second;
        outfile << "[" << segment << "]" << std::endl;
        for (auto keyit = KM.begin(); keyit != KM.end(); keyit++) {
            key = keyit->first;
            value = keyit->second;
            outfile << "\t" << key << " = " << value << std::endl;
        }
    }
    outfile.close();
}

void IniFile::save_as(const std::string& filename) {
    std::ofstream outfile(filename, std::ios::out);
    std::string segment, key, value;
    KeysMap KM;
    for (auto segit = data.begin(); segit != data.end(); segit++) {
        segment = segit->first;
        KM = segit->second;
        outfile << "[" << segment << "]" << std::endl;
        for (auto keyit = KM.begin(); keyit != KM.end(); keyit++) {
            key = keyit->first;
            value = keyit->second;
            outfile << "\t" << key << " = " << value << std::endl;
        }
    }
    outfile.close();
}



int IniFile::readInt(const std::string& section_in, const std::string& key_in, int def) {
    std::string section = section_in; ParseFromSpaces(section);
    std::string key = key_in; ParseFromSpaces(key);
    if (data.count(section) == 0) return def;
    KeysMap &KM = data[section];
    
    if (KM.count(key) == 0) return def;
    std::string value = KM[key];
    return std::stoi(value);
}

double IniFile::readDouble(const std::string& section_in, const std::string& key_in, double def) {
    std::string section = section_in; ParseFromSpaces(section);
    std::string key = key_in; ParseFromSpaces(key);
    if (data.count(section) == 0) return def;
    KeysMap &KM = data[section];
    
    if (KM.count(key) == 0) return def;
    std::string value = KM[key];
    return std::stod(value);
}

std::string IniFile::readString(const std::string& section_in, const std::string& key_in, const std::string& def) {
    std::string section = section_in; ParseFromSpaces(section);
    std::string key = key_in; ParseFromSpaces(key);
    if (data.count(section) == 0) return def;
    KeysMap &KM = data[section];
    
    if (KM.count(key) == 0) return def;
    std::string value = KM[key];
    return value;
}

bool IniFile::readBool(const std::string& section_in, const std::string& key_in, bool def) {
    std::string section = section_in; ParseFromSpaces(section);
    std::string key = key_in; ParseFromSpaces(key);
    if (data.count(section) == 0) return def;
    KeysMap &KM = data[section];
    
    if (KM.count(key) == 0) return def;
    std::string value = ConvertToLowercase(KM[key]);
    for (int i=0; i<trueValues.size(); i++) {
        if (trueValues[i] == value) return true;
    }
    return false;
}


void IniFile::writeInt(const std::string& section_in, const std::string& key_in, int value_in) {
    std::string section = section_in; ParseFromSpaces(section);
    std::string key = key_in; ParseFromSpaces(key);
    std::string value = std::to_string(value_in);
    if (data.count(section) == 0) throw std::invalid_argument("No such section: " + section);
    KeysMap &KM = data[section];
    
    if (KM.count(key) == 0) 
        KM.insert({key, value});
    else
        KM[key] = value;
}

void IniFile::writeDouble(const std::string& section_in, const std::string& key_in, double value_in) {
    std::string section = section_in; ParseFromSpaces(section);
    std::string key = key_in; ParseFromSpaces(key);
    std::string value = std::to_string(value_in);
    if (data.count(section) == 0) throw std::invalid_argument("No such section: " + section);
    KeysMap &KM = data[section];
    
    if (KM.count(key) == 0) 
        KM.insert({key, value});
    else
        KM[key] = value;
}

void IniFile::writeString(const std::string& section_in, const std::string& key_in, const std::string& value_in) {
    std::string section = section_in; ParseFromSpaces(section);
    std::string key = key_in; ParseFromSpaces(key);
    std::string value = value_in; ParseFromSpaces(value);
    if (key.empty()) {throw std::invalid_argument("Cannot write to empty key");}
    if (value.empty()) {throw std::invalid_argument("Cannot write empty value");}
    if (data.count(section) == 0) throw std::invalid_argument("No such section: " + section);
    KeysMap &KM = data[section];
    
    
    if (KM.count(key) == 0) 
        KM.insert({key, value});
    else
        KM[key] = value;
}


void IniFile::writeBool(const std::string& section_in, const std::string& key_in, bool value_in) {
    std::string section = section_in; ParseFromSpaces(section);
    std::string key = key_in; ParseFromSpaces(key);
    std::string value = std::to_string(value_in);
    if (data.count(section) == 0) throw std::invalid_argument("No such section: " + section);
    KeysMap &KM = data[section];
    
    if (KM.count(key) == 0) 
        KM.insert({key, value});
    else
        KM[key] = value;
}



bool IniFile::isSectionExist(const std::string& section_in) {
    std::string section = section_in; ParseFromSpaces(section);
    if (data.count(section)==0) return false;
    return true;
}

bool IniFile::isKeysExist(const std::string& section_in, const std::string& key_in) {
    std::string section = section_in; ParseFromSpaces(section);
    std::string key = key_in; ParseFromSpaces(key);
    if (data.count(section) == 0) throw std::invalid_argument("No such section: " + section);
    KeysMap &KM = data[section];
    
    if (KM.count(key) == 0) return false;
    return true;
}

size_t IniFile::getSectionsCount() {
    return data.size();
}


size_t IniFile::getKeysCount(const std::string& section_in) {
    std::string section = section_in; ParseFromSpaces(section);
    if (data.count(section) == 0) throw std::invalid_argument("No such section: " + section);
    return data[section].size();
}

SectionsMap IniFile::getSections() const {
    return data;
}

KeysMap IniFile::getKeys(const std::string &section_in) {
    std::string section = section_in; ParseFromSpaces(section);
    if (data.count(section) == 0) throw std::invalid_argument("No such section: " + section);
    return data[section];
}

bool IniFile::deleteSection(const std::string& section_in) {
    std::string section = section_in; ParseFromSpaces(section);
    if (data.count(section) == 0) return false;
    data[section].clear();
    data.erase(section);
    return true;
}


bool IniFile::deleteKey(const std::string& section_in, const std::string& key_in) {
    std::string section = section_in; ParseFromSpaces(section);
    std::string key = key_in; ParseFromSpaces(key);
    if (data.count(section) == 0) return false;
    KeysMap &KM = data[section];
    
    if (KM.count(key) == 0) return false;
    KM.erase(key);
    return true;
}


bool IniFile::addNewSection(const std::string& section_in) {
    std::string section = section_in; ParseFromSpaces(section);
    if (section.empty()) {return false;}
    if (data.count(section) != 0) {return false;}
    KeysMap KM;
    data.insert({section, KM});
    return true;
}


void IniFile::clear() {
    for (auto it = data.begin(); it != data.end(); it++) { 
        it->second.clear();
    }
    data.clear();
}

IniFile IniFile::merge(const IniFile& primary, const IniFile& secondary, const std::string& filePath_in) {
    std::string filePath = ParseFromSpacesReturn(filePath_in);
    if (filePath == primary.filePath || filePath == secondary.filePath) throw std::invalid_argument("New filePath must differ from filePath's of the IniFiles it is merged from");

    IniFile IniNew(filePath);
    for (auto it = primary.data.begin(); it != primary.data.end(); it++) { 
        if (IniNew.data.count(it->first) == 0) {IniNew.data.insert({it->first, it->second}); continue;}

        for (auto key = it->second.begin(); key != it->second.end(); key++) {
            IniNew.data[it->first].insert({key->first, key->second});
        }
    }
    for (auto it = secondary.data.begin(); it != secondary.data.end(); it++) { 
        if (IniNew.data.count(it->first) == 0) {IniNew.data.insert({it->first, it->second}); continue;}

        for (auto key = it->second.begin(); key != it->second.end(); key++) {
            IniNew.data[it->first].insert({key->first, key->second});
        }
    }

    return IniNew;
}

std::string IniFile::get_path() const {
    return this->filePath;
}
