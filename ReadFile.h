/*
 * ReadFile.h
 *
 *  Created on: 03/07/2016
 *      Author: Bishal Gautam
 *      UFID        : 13103195
 */

#ifndef READFILE_H_
#define READFILE_H_

class ReadFile {
private:
	char* fileName;
public:
	ReadFile(char* fileName);
	virtual ~ReadFile();
	std::string getFileContent();

};

#endif /* READFILE_H_ */
