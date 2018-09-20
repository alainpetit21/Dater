#include <iostream>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include <utime.h>

int
main(int argc, char** argv)
{
	if(argc == 1){
		printf("Wrong usage : dater \"YYYY/[MM/DD/]filename\"\n");
		return 1;
	}

	int i;
	for(i= 1; i < argc; ++i){
		char	strYear[5];
		char	strMonth[3];
		char	strDay[3];
		char	strHour[3];
		char	strMin[3];
		int 	nYear;
		int 	nMonth	= 1;
		int 	nDay	= 1;
		int		nHour;
		int		nMin;

		printf("\n%s", argv[i]);


		//Try to find the year directory this way : /19XX/
		char *pt= &argv[i][0];
		while(*pt){
			if((pt[0] == '/') && (pt[1] == '1') && (pt[2] == '9'))
				break;
			if((pt[0] == '/') && (pt[1] == '2') && (pt[2] == '0'))
				break;

			pt++;
		}
		if(!*pt){
			printf("Wrong directory struture ... your files must be in a /19YY/ or /20YY/ year directory");
			return 1;
		}

		pt++;
		memcpy(strYear, pt, 4);
		strYear[4]= 0;
		nYear= atoi(strYear);
		pt+= 5;

		//Try to find the month
		if(pt[2] == '/'){
			memcpy(strMonth, pt, 2);
			strMonth[2]= 0;
			nMonth= atoi(strMonth);
			pt+= 3;

			//Try to find the day
			if(pt[2] == '/'){
				memcpy(strDay, pt, 2);
				strDay[2]= 0;
				nDay= atoi(strDay);
				pt+= 3;
			}
		}

		//Get the Base info
		struct tm* clock;					// create a time structure
		struct stat attrib;					// create a file attribute structure

		stat(argv[i], &attrib);				// get the attributes of afile.txt
		clock= localtime(&(attrib.st_mtime));	// Get the last modified time and put it into the time structure

		clock->tm_year= nYear - 1900;
		clock->tm_mon= nMonth - 1;
		clock->tm_mday= nDay;


		struct utimbuf timeToWrite;

		timeToWrite.actime = mktime(clock);
		timeToWrite.modtime = mktime(clock);

		utime(argv[i], &timeToWrite);
	}

	return 0;
}
