#include <iostream>
#include <string>
#include <clocale>
#include <time.h>
#include <stdlib.h>
#include <fstream>

using namespace std;


#define MAX_SAYI 15

// Finansal veriler icin yapi
struct Kayit {
    int islem_id;
    string aciklama;
    string tur; // Gelir veya Gider
    double miktar;
};

// Global degiskenler
Kayit liste[MAX_SAYI];
int kayit_sayisi = 0;

// Program ilk acildiginda yuklenecek veriler
string hazir_islem[] = {"Aidat Odeme", "Havuz Bakim", "Bahce Ilaclama", "Ortak Elektrik", "Kira Geliri"};
string hazir_tur[] = {"Gelir", "Gider", "Gider", "Gider", "Gelir"};

void dosyaya_kaydet() {
    ofstream dosya("yonetim_kayitlari.txt");
    if (dosya.is_open()) {
        dosya << "ID | Aciklama | Tur | Tutar" << endl;
        for (int i = 0; i < kayit_sayisi; i++) {
            // Sadece silinmemis kayitlari dosyaya 
            if (liste[i].aciklama != "-NULL-") {
                dosya << liste[i].islem_id << " | " << liste[i].aciklama << " | " << liste[i].tur << " | " << liste[i].miktar << " TL" << endl;
            }
        }
        dosya.close();
    }
}

void verileri_yukle() {
    srand(time(0));
    for (int i = 0; i < 5; i++) {
        liste[i].aciklama = hazir_islem[i];
        liste[i].tur = hazir_tur[i];
        liste[i].islem_id = 1000 + i;
        liste[i].miktar = (rand() % 500) + 200;
        kayit_sayisi++;
    }
}

void yeni_islem_ekle() {
    system("cls");
    if (kayit_sayisi >= MAX_SAYI) {
        cout << "Liste dolu, daha fazla kayit eklenemez!\n";
        return;
    }
    
    bool yer_doldu = false;
    int s_id;
    string s_aciklama, s_tur;
    double s_miktar;

    cout << "\n--- Yeni Finansal Kayit ---\n";
    cout << "Islem ID: "; cin >> s_id;
    cin.ignore();
    cout << "Aciklama: "; getline(cin, s_aciklama);
    cout << "Tur (Gelir/Gider): "; getline(cin, s_tur);
    cout << "Miktar: "; cin >> s_miktar;

    // Once silinmis bir yer var mi diye kontrol etsin(Berke cok temkinlidir)
    for (int a = 0; a < kayit_sayisi; a++) {
        if (liste[a].aciklama == "-NULL-") {
            liste[a].islem_id = s_id;
            liste[a].aciklama = s_aciklama;
            liste[a].tur = s_tur;
            liste[a].miktar = s_miktar;
            yer_doldu = true; // Mevcut bosluk dolduruldu, sayac artmaz
            break; 
        }
    }

    // Eger hic bosluk yoksa, listenin sonuna ekle ve sayaci arttir(Ne zor odev arkadas)
    if (!yer_doldu) {
        liste[kayit_sayisi].islem_id = s_id;
        liste[kayit_sayisi].aciklama = s_aciklama;
        liste[kayit_sayisi].tur = s_tur;
        liste[kayit_sayisi].miktar = s_miktar;
        kayit_sayisi++;
    }

    cout << "Islem basariyla listeye eklendi.\n";
    dosyaya_kaydet();
}

void islem_ara(string isim) {
    system("cls");
    bool bulundu = false;
    if (isim == "") {
        cin.ignore(1000, '\n');
        cout << "\nAranacak Aciklamayi Girin: ";
        getline(cin, isim);
    }

    for (int k = 0; k < kayit_sayisi; k++) {
        if (isim == liste[k].aciklama && liste[k].aciklama != "-NULL-") {
            cout << "\n>>> KAYIT BULUNDU <<<" << endl;
            cout << "ID: " << liste[k].islem_id << endl;
            cout << "Detay: " << liste[k].aciklama << endl;
            cout << "Tur: " << liste[k].tur << endl;
            cout << "Tutar: " << liste[k].miktar << " TL" << endl;
            bulundu = true;
            break; 
        }
    }

    if (!bulundu) cout << "\n'" << isim << "' isimli bir kayit yok.\n";
}

void listeyi_dok() {
    system("cls");
    double toplam_bakiye = 0;
    cout << "\nSitenin Guncel Finansal Durumu:\n";
    cout << "----------------------------------------------\n";

    for (int i = 0; i < kayit_sayisi; i++) {
        // Silinmemis kayitlari goster
        if (liste[i].aciklama != "-NULL-") {
            cout << "ID: " << liste[i].islem_id << " | " << liste[i].aciklama << " | " << liste[i].tur << " : " << liste[i].miktar << " TL" << endl;
            
            if (liste[i].tur == "Gelir") {
                toplam_bakiye += liste[i].miktar;
            } else {
                toplam_bakiye -= liste[i].miktar;
            }
        }
    }
    cout << "----------------------------------------------\n";
    cout << "Kasadaki Toplam: " << toplam_bakiye << " TL" << endl;
}

void islem_sil(string isim) {
    system("cls");
    bool silindi = false;
    if (isim == "") {
        cin.ignore(1000, '\n');
        cout << "\nSilinecek Islemin Aciklamasi: ";
        getline(cin, isim);
    }

    for (int d = 0; d < kayit_sayisi; d++) {
        if (isim == liste[d].aciklama && liste[d].aciklama != "-NULL-") {
            // Silme islemini verileri sifirlayarak yapýyoruz(Hata çýkarsa kafayi yerým)
            liste[d].aciklama = "-NULL-";
            liste[d].tur = "-NULL-";
            liste[d].islem_id = 0;
            liste[d].miktar = 0;
            silindi = true;
            break; 
        }
    }
    
    if (silindi) {
        cout << "\nKayit basariyla silindi." << endl;
        
        dosyaya_kaydet();
    } else {
        cout << "\nAranan kayit bulunamadigi icin silme yapilamadi!" << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Turkish");
    cout << "Sistem yukleniyor...";
    system("pause");

    verileri_yukle();
    
    int secim = 0;
    while (secim != 5) {
        cout << "\n1. Yeni Kayit | 2. Islem Ara | 3. Listeyi Gor | 4. Kayit Sil | 5. Cikis\nSeciminiz: ";
        cin >> secim;

        switch (secim) {
            case 1: yeni_islem_ekle(); break;
            case 2: islem_ara(""); break;
            case 3: listeyi_dok(); break;
            case 4: islem_sil(""); break;
        }
    }

    dosyaya_kaydet();
    return 0;
}
