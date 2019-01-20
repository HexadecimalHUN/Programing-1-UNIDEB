// z3a7.cpp
//
// Együtt támadjuk meg: http://progpater.blog.hu/2011/04/14/egyutt_tamadjuk_meg
// LZW fa építő 3. C++ átirata a C valtozatbol (+mélység, atlag és szórás)
// Programozó Páternoszter
//
// Copyright (C) 2011, 2012, Bátfai Norbert, nbatfai@inf.unideb.hu, nbatfai@gmail.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Ez a program szabad szoftver; terjeszthetõ illetve módosítható a
// Free Software Foundation által kiadott GNU General Public License
// dokumentumában leírtak; akár a licenc 3-as, akár (tetszõleges) késõbbi
// változata szerint.
//
// Ez a program abban a reményben kerül közreadásra, hogy hasznos lesz,
// de minden egyéb GARANCIA NÉLKÜL, az ELADHATÓSÁGRA vagy VALAMELY CÉLRA
// VALÓ ALKALMAZHATÓSÁGRA való származtatott garanciát is beleértve.
// További részleteket a GNU General Public License tartalmaz.
//
// A felhasználónak a programmal együtt meg kell kapnia a GNU General
// Public License egy példányát; ha mégsem kapta meg, akkor
// tekintse meg a <http://www.gnu.org/licenses/> oldalon.
//
//
// Version history:
//
// 0.0.1,       http://progpater.blog.hu/2011/02/19/gyonyor_a_tomor
// 0.0.2,       csomópontok mutatóinak NULLázása (nem fejtette meg senki :)
// 0.0.3,       http://progpater.blog.hu/2011/03/05/labormeres_otthon_avagy_hogyan_dolgozok_fel_egy_pedat
// 0.0.4,       z.cpp: a C verzióból svn: bevezetes/C/ziv/z.c átírjuk C++-ra
//              http://progpater.blog.hu/2011/03/31/imadni_fogjatok_a_c_t_egy_emberkent_tiszta_szivbol
// 0.0.5,       z2.cpp: az fgv(*mut)-ok helyett fgv(&ref)
// 0.0.6,       z3.cpp: Csomopont beágyazva
//              http://progpater.blog.hu/2011/04/01/imadni_fogjak_a_c_t_egy_emberkent_tiszta_szivbol_2
// 0.0.6.1      z3a2.c: LZWBinFa már nem barátja a Csomopont-nak, mert annak tagjait nem használja direktben
// 0.0.6.2      Kis kommentezést teszünk bele 1. lépésként (hogy a kicsit lemaradt hallgatóknak is
//              könnyebb legyen, jól megtűzdeljük további olvasmányokkal)
//              http://progpater.blog.hu/2011/04/14/egyutt_tamadjuk_meg
//              (majd a 2. lépésben "beletesszük a d.c-t", majd s 3. lépésben a parancssorsor argok feldolgozását)
// 0.0.6.3      z3a2.c: Fejlesztgetjük a forrást: http://progpater.blog.hu/2011/04/17/a_tizedik_tizenegyedik_labor
// 0.0.6.4      SVN-beli, http://www.inf.unideb.hu/~nbatfai/p1/forrasok-SVN/bevezetes/vedes/
// 0.0.6.5      2012.03.20, z3a4.cpp: N betűk (hiányok), sorvégek, vezető komment figyelmen kívül: http://progpater.blog.hu/2012/03/20/a_vedes_elokeszitese
// 0.0.6.6      z3a5.cpp: mamenyaka kolléga észrevételére a több komment sor figyelmen kívül hagyása
//		http://progpater.blog.hu/2012/03/20/a_vedes_elokeszitese/fullcommentlist/1#c16150365
// 0.0.6.7	Javaslom ezt a verziót választani védendő programnak
// 0.0.6.8
// 0.0.7        z3a7.cpp: pár kisebb javítás, illetve a védések támogatásához további komment a <<
// 		eltoló operátort tagfüggvényként, illetve globális függvényként túlterhelő részekhez.
//		http://progpater.blog.hu/2012/04/10/imadni_fogjak_a_c_t_egy_emberkent_tiszta_szivbol_4/fullcommentlist/1#c16341099
// 0.0.8        z3a8.cpp: alapban nocopyable volt a fa és a csomópont, de most a fát kiveszem a
//              privátból, hogy lehessen állatorvosi ló és írok a fához egy copy ctor-t
// 0.0.9	z3a9.cpp: alapban nocopyable volt a fa és a csomópont, de most a fát kiveszem a... mozgató
//              ctor a fához, hogy a lemaradó 2. védések kapcsán tudjunk beszélgetni a védésen a
//              mozgató szemantikáról, ugyancsak állatorvosi ló megközelítéssel
//

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>


class LZWBinFa {
public:

     LZWBinFa () :fa ( &gyoker ) {

     }
     ~LZWBinFa () {
          std::cout << "LZWBinFa dtor" << std::endl;
          szabadit ( gyoker.egyesGyermek () );
          szabadit ( gyoker.nullasGyermek () );
     }

     LZWBinFa ( const LZWBinFa & regi ) {
          std::cout << "Belépés a másoló konstruktorba.." << std::endl;

          gyoker.ujEgyesGyermek ( masol ( regi.gyoker.egyesGyermek (), regi.fa ) );
          gyoker.ujNullasGyermek ( masol ( regi.gyoker.nullasGyermek (), regi.fa ) );

          if ( regi.fa == & ( regi.gyoker ) )
               fa = &gyoker;

          std::cout << "Másolás végrehajtva." << std::endl;

     }



     LZWBinFa& operator<< ( char b ) {
          if ( b == '0' ) {
               /* Van '0'-s gyermeke az aktuális csomópontnak?
                  megkérdezzük Tőle, a "fa" mutató éppen reá mutat */
               if ( !fa->nullasGyermek () ) {	// ha nincs, hát akkor csinálunk
                    // elkészítjük, azaz páldányosítunk a '0' betű akt. parammal
                    Csomopont *uj = new Csomopont ( '0' );
                    // az aktuális csomópontnak, ahol állunk azt üzenjük, hogy
                    // jegyezze már be magának, hogy nullás gyereke mostantól van
                    // küldjük is Neki a gyerek címét:
                    fa->ujNullasGyermek ( uj );
                    // és visszaállunk a gyökérre (mert ezt diktálja az alg.)
                    fa = &gyoker;
               } else {		// ha van, arra rálépünk
                    // azaz a "fa" pointer már majd a szóban forgó gyermekre mutat:
                    fa = fa->nullasGyermek ();
               }
          }
          // Mit kell betenni éppen, vagy '1'-et?
          else {
               if ( !fa->egyesGyermek () ) {
                    Csomopont *uj = new Csomopont ( '1' );
                    fa->ujEgyesGyermek ( uj );
                    fa = &gyoker;
               } else {
                    fa = fa->egyesGyermek ();
               }
          }

          return *this;
     }

     void kiir ( void ) {

          melyseg = 0;
          kiir ( &gyoker, std::cout );
     }




     int getMelyseg ( void );
     double getAtlag ( void );
     double getSzoras ( void );

     friend std::ostream & operator<< ( std::ostream & os, LZWBinFa & bf ) {
          bf.kiir ( os );
          return os;
     }
     void kiir ( std::ostream & os ) {
          melyseg = 0;
          kiir ( &gyoker, os );
     }

private:
     class Csomopont {
     public:

          Csomopont ( char b = '/' ) :betu ( b ), balNulla ( 0 ), jobbEgy ( 0 ) {
          };
          ~Csomopont () {};

          Csomopont *nullasGyermek () const {
               return balNulla;
          }

          Csomopont *egyesGyermek () const {
               return jobbEgy;
          }

          void ujNullasGyermek ( Csomopont * gy ) {
               balNulla = gy;
          }

          void ujEgyesGyermek ( Csomopont * gy ) {
               jobbEgy = gy;
          }

          char getBetu () const {
               return betu;
          }

     private:


          char betu;
          Csomopont *balNulla;
          Csomopont *jobbEgy;
          // nem másolható a csomópont (ökörszabály: ha van valamilye a szabad tárban,
          // letiltjuk a másoló konstruktort, meg a másoló értékadást)
          Csomopont ( const Csomopont & );
          Csomopont & operator= ( const Csomopont & );

     };

     Csomopont *fa;

     int melyseg, atlagosszeg, atlagdb;
     double szorasosszeg;

     /*
     // szokásosan: nocopyable
     LZWBinFa (const LZWBinFa &);
     LZWBinFa & operator= (const LZWBinFa &);
     */

     void kiir ( Csomopont * elem, std::ostream & os ) {
          if ( elem != NULL ) {
               ++melyseg;
               kiir ( elem->egyesGyermek (), os );
               kiir ( elem->nullasGyermek (), os );

               for ( int i = 0; i < melyseg; ++i )
                    os << "---";

               os << elem->getBetu () << "(" << melyseg - 1 << ")" << std::endl;
               --melyseg;
          }
     }



     void szabadit ( Csomopont * elem ) {
          if ( elem != NULL ) {
               szabadit ( elem->egyesGyermek () );
               szabadit ( elem->nullasGyermek () );
               delete elem;
          }
     }

     // a copy ctor hívja
     Csomopont *  masol ( Csomopont * elem, Csomopont * regifa ) {

          Csomopont * ujelem = NULL;

          if ( elem != NULL ) {
               ujelem = new Csomopont ( elem->getBetu() );

               ujelem->ujEgyesGyermek ( masol ( elem->egyesGyermek (), regifa ) );
               ujelem->ujNullasGyermek ( masol ( elem->nullasGyermek (), regifa ) );

               if ( regifa == elem )
                    fa = ujelem;

          }

          return ujelem;
     }

protected:

     Csomopont gyoker;
     int maxMelyseg;
     double atlag, szoras;

     void rmelyseg ( Csomopont * elem );
     void ratlag ( Csomopont * elem );
     void rszoras ( Csomopont * elem );

};


int LZWBinFa::getMelyseg ( void )
{
     melyseg = maxMelyseg = 0;
     rmelyseg ( &gyoker );
     return maxMelyseg - 1;
}

double LZWBinFa::getAtlag ( void )
{
     melyseg = atlagosszeg = atlagdb = 0;
     ratlag ( &gyoker );
     atlag = ( ( double ) atlagosszeg ) / atlagdb;
     return atlag;
}

double LZWBinFa::getSzoras ( void )
{
     atlag = getAtlag ();
     szorasosszeg = 0.0;
     melyseg = atlagdb = 0;

     rszoras ( &gyoker );

     if ( atlagdb - 1 > 0 )
          szoras = std::sqrt ( szorasosszeg / ( atlagdb - 1 ) );
     else
          szoras = std::sqrt ( szorasosszeg );

     return szoras;
}

void LZWBinFa::rmelyseg ( Csomopont * elem )
{
     if ( elem != NULL ) {
          ++melyseg;
          if ( melyseg > maxMelyseg )
               maxMelyseg = melyseg;
          rmelyseg ( elem->egyesGyermek () );
          // ez a postorder bejáráshoz képest
          // 1-el nagyobb mélység, ezért -1
          rmelyseg ( elem->nullasGyermek () );
          --melyseg;
     }
}

void LZWBinFa::ratlag ( Csomopont * elem )
{
     if ( elem != NULL ) {
          ++melyseg;
          ratlag ( elem->egyesGyermek () );
          ratlag ( elem->nullasGyermek () );
          --melyseg;
          if ( elem->egyesGyermek () == NULL && elem->nullasGyermek () == NULL ) {
               ++atlagdb;
               atlagosszeg += melyseg;
          }
     }
}

void LZWBinFa::rszoras ( Csomopont * elem )
{
     if ( elem != NULL ) {
          ++melyseg;
          rszoras ( elem->egyesGyermek () );
          rszoras ( elem->nullasGyermek () );
          --melyseg;
          if ( elem->egyesGyermek () == NULL && elem->nullasGyermek () == NULL ) {
               ++atlagdb;
               szorasosszeg += ( ( melyseg - atlag ) * ( melyseg - atlag ) );
          }
     }
}


/* Ez volt eddig a main, de most komplexebb kell, mert explicite bejövő, kimenő fájlokkal kell dolgozni
int
main ()
{
    char b;
    LZWBinFa binFa;

    while (std::cin >> b)
    {
        binFa << b;
    }

    //std::cout << binFa.kiir (); // így rajzolt ki a fát a korábbi verziókban de, hogy izgalmasabb legyen
    // a példa, azaz ki lehessen tolni az LZWBinFa-t kimeneti csatornára:

    std::cout << binFa; // ehhez kell a globális operator<< túlterhelése, lásd fentebb

    std::cout << "depth = " << binFa.getMelyseg () << std::endl;
    std::cout << "mean = " << binFa.getAtlag () << std::endl;
    std::cout << "var = " << binFa.getSzoras () << std::endl;

    binFa.szabadit ();

    return 0;
}
*/

/* A parancssor arg. kezelést egyszerűen bedolgozzuk a 2. hullám kapcsolódó feladatából:
 http://progpater.blog.hu/2011/03/12/hey_mikey_he_likes_it_ready_for_more_3
 de mivel nekünk sokkal egyszerűbb is elég, alig hagyunk meg belőle valamit...
 */

void
usage ( void )
{
     std::cout << "Usage: lzwtree in_file -o out_file" << std::endl;
}

// 0.0.8, az állatorvosi tesztelésekhez a védéseken
void
fgv ( LZWBinFa binFa )
{
     binFa << '1';

     std::cout << binFa;

     std::cout << "depth = " << binFa.getMelyseg () << std::endl;
     std::cout << "mean = " << binFa.getAtlag () << std::endl;
     std::cout << "var = " << binFa.getSzoras () << std::endl;
}

int
main ( int argc, char *argv[] )
{
     // http://progpater.blog.hu/2011/03/12/hey_mikey_he_likes_it_ready_for_more_3
     // alapján a parancssor argok ottani elegáns feldolgozásából kb. ennyi marad:
     // "*((*++argv)+1)"...

     // a kiírás szerint ./lzwtree in_file -o out_file alakra kell mennie, ez 4 db arg:
     if ( argc != 4 ) {
          // ha nem annyit kapott a program, akkor felhomályosítjuk erről a júzetr:
          usage ();
          // és jelezzük az operációs rendszer felé, hogy valami gáz volt...
          return -1;
     }

     // "Megjegyezzük" a bemenő fájl nevét
     char *inFile = *++argv;

     // a -o kapcsoló jön?
     if ( * ( ( *++argv ) + 1 ) != 'o' ) {
          usage ();
          return -2;
     }

     // ha igen, akkor az 5. előadásból kimásoljuk a fájlkezelés C++ változatát:
     std::fstream beFile ( inFile, std::ios_base::in );

     // fejlesztgetjük a forrást: http://progpater.blog.hu/2011/04/17/a_tizedik_tizenegyedik_labor
     if ( !beFile ) {
          std::cout << inFile << " nem letezik..." << std::endl;
          usage ();
          return -3;
     }

     std::fstream kiFile ( *++argv, std::ios_base::out );

     unsigned char b;		// ide olvassik majd a bejövő fájl bájtjait
     LZWBinFa binFa;		// s nyomjuk majd be az LZW fa objektumunkba

     // a bemenetet binrisan olvassuk, de a kimenő fájlt már karakteresen írjuk, hogy meg tudjuk
     // majd nézni... :) l. az említett 5. ea. C -> C++ gyökkettes átírási példáit

     // 0.0.8, az állatorvosi tesztelésekhez a védéseken ki a humán genom fájlból olvasását és
     // pár betűt kézzel nyomunk majd alább a fába
     while (beFile.read ((char *) &b, sizeof (unsigned char)))
       {
/*
         if (b == 0x3e)
     {			// > karakter
       kommentben = true;
       continue;
     }

         if (b == 0x0a)
     {			// újsor
       kommentben = false;
       continue;
     }

         if (kommentben)
     continue;

         if (b == 0x4e)		// N betű
     continue;

         // egyszerűen a korábbi d.c kódját bemásoljuk
         // laboron többször lerajzoltuk ezt a bit-tologatást:
         // a b-ben lévő bájt bitjeit egyenként megnézzük
  */       for (int i = 0; i < 8; ++i)
     {
       // maszkolunk eddig..., most már simán írjuk az if fejébe a legmagasabb helyiértékű bit vizsgálatát
       // csupa 0 lesz benne a végén pedig a vizsgált 0 vagy 1, az if megmondja melyik:
       if (b == 0x80)
         // ha a vizsgált bit 1, akkor az '1' betűt nyomjuk az LZW fa objektumunkba
         binFa << '1';
       else
         // különben meg a '0' betűt:
         binFa << '0';
       b <<= 1;
     }

       }

     //std::cout << binFa.kiir (); // így rajzolt ki a fát a korábbi verziókban de, hogy izgalmasabb legyen
     // a példa, azaz ki lehessen tolni az LZWBinFa-t kimeneti csatornára:


     fgv ( binFa );




     kiFile << binFa;   // ehhez kell a globális operator<< túlterhelése, lásd fentebb
     // (jó ez az OO, mert mi ugye nem igazán erre gondoltunk, amikor írtuk, mégis megy, hurrá)

     kiFile << "depth = " << binFa.getMelyseg () << std::endl;
     kiFile << "mean = " << binFa.getAtlag () << std::endl;
     kiFile << "var = " << binFa.getSzoras () << std::endl;

     LZWBinFa binFa3 = binFa;
     kiFile << "Másolt binfa értékei:" << std::endl;
     kiFile << "depth = " << binFa3.getMelyseg () << std::endl;
     kiFile << "mean = " << binFa3.getAtlag () << std::endl;
     kiFile << "var = " << binFa3.getSzoras () << std::endl;

     kiFile.close ();
     beFile.close ();



     return 0;
}
