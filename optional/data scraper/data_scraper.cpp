#include <iostream>
#include <string>
#include <curl/curl.h>
#include <fstream>
#include <ctime>
#include <chrono>
#include <thread>  
#include <vector>
#include <algorithm>       


using namespace std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void get_data(string url, string name) {
	CURL *curl;
    CURLcode res;
    string readBuffer;

    curl = curl_easy_init();
	
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        cout << readBuffer << std::endl;
    }
	
	
    string filename = "/home/pi/Hydro/" + name + ".txt"; //Change this to your path
 	
    ofstream data(filename, ios_base::app);
    if(!data) {
        cout << "cannot open the output file." << endl;
    }
    auto timenow =  chrono::system_clock::to_time_t(chrono::system_clock::now());
    string final_data = readBuffer + " : " + ctime(&timenow);
    data<< final_data;
    data.close();


}

int day (int &d){ //get current day
	time_t tt; 
    struct tm * ti; 
    time (&tt); 
    ti = localtime(&tt);     
    string date = asctime(ti);  
    date.erase (date.begin()+10, date.end());
    date.erase (date.begin(), date.end()-2);  	
	d = stoi(date);
	return d;
}

void mittelwert(string name){
	
	string filename = "/home/pi/Hydro/" + name + ".txt";
	vector <float> werte;
	float m;
	
	fstream f;
	f.open(filename, ios::in);
	if(!f){
		cout << "No such File";
	}
	else {
		string chara;
		while (getline(f, chara)) {
			
			string chara_v;
		
			
			//Only Values
			for (int i = 0; i < chara.length(); i++ ){
				if (chara[i] == ' '){
					chara.erase(chara.begin()+i, chara.end());
					chara_v = chara;				
					
				}
				
			}
			
			//cout << chara_v << endl;
			werte.push_back(stof(chara_v));
		
			if (f.eof())
				break;
		}
		
		//Mittelwert
		for (int i = 0; i < werte.size(); i++){
			m += werte[i];
		}
		m = m / werte.size();
		
		cout << m << endl;
		
		
	}
	
	string out = "/home/pi/Hydro/" + name + "_days.txt";
	
    ofstream data(out, ios_base::app);
    if(!data) {
        cout << "cannot open the output file." << endl;
    }
    auto timenow =  chrono::system_clock::to_time_t(chrono::system_clock::now());
	string time = ctime(&timenow);
	time.erase (time.begin()+10, time.end());
    	
    string final_data = to_string(m) + " : " + time;
    data<< final_data;
    data.close();
	
	
	
	// Empy old file
	
	string old = "/home/pi/Hydro/" + name + ".txt";
	ofstream ofs;
	ofs.open(old, std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	
}

int main(void)
{
	auto current_time =  chrono::system_clock::to_time_t(chrono::system_clock::now());
	int date_save;
	int tag;
	
	date_save = day(date_save);
	
	
	while (true){
	tag = day(tag);
	
	if(tag == date_save){
	
	get_data("http://196.254.157.86/waterlevel", "Wasserstand");
	get_data("http://196.254.157.86/temperature", "Temperatur");
	get_data("http://196.254.157.86/humidity", "Humidity");
	get_data("http://196.254.157.86/watertemp", "Watertemp");
	get_data("http://196.254.157.86/co2", "Co2");
	
	this_thread::sleep_for (chrono::seconds(60));
	
	}
	else if(tag != date_save){
		mittelwert("Wasserstand");
		mittelwert("Temperatur");
		mittelwert("Humidity");
		mittelwert("Watertemp");
		mittelwert("Co2");

		date_save = tag;
		
	}
	
	cout << tag << " Ready" << endl;
       	cout << date_save << " | " << tag <<  endl; 	
	}
	

    return 0;
}
