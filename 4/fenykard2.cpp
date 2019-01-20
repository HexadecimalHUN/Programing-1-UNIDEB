// compile g++ fenykard2.cpp -o fenykard2 -std=c++11 /usr/lib/x86_64-linux-gnu/libboost_filesystem.a -lboost_filesystem -lboost_system -fpermissive
// futas ./fenykard2 input opcio ahol opcio = {pont, vedes}
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <experimental/filesystem>
#include <sys/stat.h>
#include <boost/token_iterator.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/bimap.hpp>
#include <iomanip>
using namespace std;
using namespace boost;
namespace fs = std::experimental::filesystem;
bool fileExists(const std::string& filename);

int jegy(string osztalyzat)
{
  if(osztalyzat == "jeles")
    return 5;
  else {
    if(osztalyzat == "jó")
      return 4;
    else {
      if(osztalyzat == "közepes")
	return 3;
      else {
	if(osztalyzat == "elégséges")
	  return 2;
	else {
	  if (osztalyzat == "elégtelen")
	  return 1;
	}
      }
    }
  }
  return 0;
}
int main(int argc, char *argv[])
{
  if(argc == 1 || argc == 2)
  {
    cout << "hasznalat ./fenykard2 input opcio ||| ahol opcio = {pont {melle egy szam ha toplistat szeretnenk}, vedes, egy future felhasznalo neve }\n";
    return -1;
  }
  if (argv[1] == std::string("help"))
  {
    cout << "hasznalat ./fenykard2 input opcio ||| ahol opcio = {pont {melle egy szam ha toplistat szeretnenk}, vedes, egy future felhasznalo neve }\n";
    return -1;
  }
  ifstream beolvas (argv[1]);
  string sor;
      int count = 0;
  bool esport = false;
  std::map<std::string, unsigned int> ember;
  std::map<int, string> rendez;
  std::map<string,vector<double>> vedesek;
  std::map<vector<double>,string> forditottvedes;
  std::multimap<pair<int,int>,string> minden;
  std::multimap<string,pair<string,int>> feladatok;

  map<string,double> vedesjegy;
  std::multimap<std::vector<double>,string> fmultivedes;
  multimap<int,string> fordiottpont;
  map<string,map<int,int>> abszolut;
  map<map<int,int>,string> abszolut2;
  typedef boost::bimap< map<int,int>, std::string > bm_type;
  bm_type bm;


  vector <string> vedes;
  int ertek = 0;
  bool ved1 = false, ved2 = false , ved3 = false;
  while(getline(beolvas,sor))
  {
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep("\",");
    tokenizer tokens(sor, sep);
    tokenizer::iterator it1, it2 = tokens.begin();
    it1 = it2;
    string nev = *it1;
    it2 ++;
    string props;
    string hely = *it2;

    string eredeti = hely;
    string last;
    boost::char_separator<char> sep2("/");
    tokenizer tokens2 (hely,sep2);
    tokenizer::iterator it3;
    vedes.clear();
    for (it3 = tokens2.begin();
      it3 != tokens2.end(); ++it3)
      {
	 vedes.push_back(*it3);
	 last = *it3;
	 if(last == "Sport")
	 {
	   esport = true;
	   break;
	 }
      }
      if(esport == true){
	esport = false;
	vedes.clear();
	continue;
      }
      ved1 = true, ved2 = true , ved3 = true;
     int szam = 0;
     if(vedes.size() == 12)
     {
      if(vedes[9] == "Védés" && vedes[8] == "Labor" && vedesek[*it1].size() < 3)
      {



       vedesek[*it1].push_back(0);
       vedesek[*it1].push_back(0);
       vedesek[*it1].push_back(0);
      }
      if( (vedes[0] == ".") && (vedes[1] == "City") && (vedes[2] == "Debrecen") &&
	  (vedes[3] == "Oktatás") && (vedes[4] == "Informatika") && (vedes[5] =="Programozás") &&
	  (vedes[6] == "DEIK") && (vedes[7] == "Prog1") && (vedes[8] == "Labor" )&& (vedes[9] == "Védés"))
	{
	if(vedes[10] == "I")
	{
	 if((szam = jegy(vedes[11])) > vedesek[*it1][0])
	 {

	   vedesek[*it1][0] = szam;
	 }
	}

	if(vedes[10] == "II")
	{

	  if((szam = jegy(vedes[11])) > vedesek[*it1][1])
	  {
	   vedesek[*it1][1] = szam;
	  }
	}

	if(vedes[10] == "III")
	{
	  if((szam = jegy(vedes[11])) > vedesek[*it1][2])
	  {
	   vedesek[*it1][2] = szam;

	  }
	}
      }
     }
     else{
     vedes.clear();
     }

   hely += "/" + last + ".props";

    string pontsor;
    bool letezik = fileExists(hely);
    if (letezik==true)
    {
      ifstream pont(hely);
      int pontszamfeladat = 0;
      string helyipont ;
      while(getline(pont,pontsor))
      {

	boost::char_separator<char> sep3("   \t");
	tokenizer tokens3(pontsor, sep3);
	for(tokenizer::iterator it4 = tokens3.begin(); it4 != tokens3.end(); it4++)
	{
	   helyipont = *it4;
	}
	pontszamfeladat += stol(helyipont);

      }
      ember[nev] += pontszamfeladat;
      feladatok.insert( make_pair( *it1,make_pair(*it2,pontszamfeladat)));
      pontszamfeladat = 0;
    }

  }
  for(auto const value: ember)
  {
    fordiottpont.insert(make_pair(value.second,value.first));
  }

   // abc szerinti

    for (const auto& pair : vedesek)
	{
		double atlag = 0;
		double szamolo = 0;

		for (std::size_t i = 0; i < pair.second.size(); ++i){
		  if(pair.second[i] != 0)
		    szamolo++;
		  atlag += pair.second[i];
		}
		double vedesatlag = (double)atlag / (double)szamolo;
		//if((vedesatlag-(int)vedesatlag) > 0.5) {
		//  abszolut[pair.first][szamolo] = ceil(vedesatlag);
	//	  minden.insert( make_pair(make_pair(szamolo,ceil(vedesatlag)), pair.first));
	//	}
	//	else
	//	{
		  abszolut[pair.first][szamolo] = floor(vedesatlag);
		  minden.insert( make_pair(make_pair(szamolo,floor(vedesatlag)), pair.first));
		//}
		
	}

    /* abc kiiras
    for (auto it : abszolut)
    {
      cout << it.first << " : ";
      map<int, int> &internal_map = it.second;
      for (auto it2: internal_map)
      {
        cout << it2.first << ":" << it2.second;
      }
    cout << endl;
    }
    */


 if (argv[2] == std::string("vedes"))
   {
    for (auto& iter : boost::adaptors::reverse(minden)) {
      std::cout <<iter.second << ": " << iter.first.first << " " << iter.first.second <<  std::endl;
    }
   }

  /* if (argv[2] == std::string("pont"))
   {


    for (auto iter = rendez.rbegin(); iter != rendez.rend(); ++iter) {
        std::cout << iter->second << ": "<< iter->first << std::endl;
      }

   }
   */
   if (argv[2] == std::string("pont"))
   {
     int szamolo2 = 1;
      if(argc == 3)
      {
	  for (auto& iter : boost::adaptors::reverse(fordiottpont)) {
	   std::cout << setw(3) <<szamolo2 << ". " << iter.second << ": "  << " " << iter.first <<  std::endl;
     szamolo2++;
	  }
       }

      	else{ if(argc == 4) {

	 string szam = argv[3];
	 int szamocska = stoi(szam);
	 int count = 0;
	 for (auto& iter : boost::adaptors::reverse(fordiottpont)) {
	    std::cout << setw(3) << count+1<<". "<<iter.second << ": "  << " " << iter.first <<  std::endl;
	     count++;
	     if(count == szamocska)
	        break;
         }
      }

   }
   }
   if(argv[2] != std::string("vedes") && argv[2] != std::string("pont"))
   {
	int osszeg = 0;
	for(auto const &elem : feladatok)
	  {
	    string emberecske = argv[2];
	    string temp;
	    int mennyivanmeg = 3;
	    while(mennyivanmeg != argc)
	    {
		temp = argv[mennyivanmeg];
		emberecske += " " + temp;
		mennyivanmeg++;
	    }

   if(emberecske == elem.first)
    {
     cout << elem.second.first << " " << elem.second.second << "\n";
     osszeg += elem.second.second;
    }

  }
  if(osszeg != 0)
  cout << "osszesen = " << osszeg << "\n";
  else
    cout << "HIBA: a nev hibas, vagy az illeto nem jelolt be semmilyen olyan tevekenyseget amely pontot erne\n";
  }
  // boostal kiirva
  /*
  for (auto& iter : boost::adaptors::reverse(rendez)) {
    std::cout << iter.second << ": " << iter.first << std::endl;
  }
  */
}

bool fileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}
