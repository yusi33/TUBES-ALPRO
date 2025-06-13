#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <conio.h>

using namespace std;

const float TARIF_PER_KM = 5000.00;
vector<string> lokasiList = {"Terminal", "Stasiun Wonokromo", "FEB Unesa", "Tel-U", "Royal", "SWK", "Kodam"};
int jarakMatrix[7][7] = {
    {0, 3, 5, 6, 4, 10, 8},
    {3, 0, 2, 5, 6, 11, 9},
    {5, 2, 0, 3, 5, 9, 7},
    {6, 5, 3, 0, 4, 8, 6},
    {4, 6, 5, 4, 0, 7, 5},
    {10,11, 9, 8, 7, 0, 3},
    {8, 9, 7, 6, 5, 3, 0}
};
vector<string> driverList = {"Budi", "ipul", "Joko", "Ambon", "Rizal", "Black", "Eka"};

struct Order {
    string pelanggan;
    string driver;
    string asal;
    string tujuan;
    float jarak;
    float biaya;
    string waktu;
    bool selesai;
};

Order* orderAktif = nullptr;

string getWaktu() {
    time_t now = time(0);
    char* dt = ctime(&now);
    return string(dt);
}

string getRandomDriver() {
    return driverList[rand() % driverList.size()];
}

int pilihLokasi(const string& label) {

    cout << "\nPilih Lokasi " << label << ":\n";
    cout << "+----+---------------------+\n";
    cout << "| No |      Lokasi         |\n";
    cout << "+----+---------------------+\n";
    
    for (int i = 0; i < lokasiList.size(); ++i) {
        cout << "| " << setw(2) << left << i + 1 << " | " 
             << setw(19) << left << lokasiList[i] << " |\n";
    }
    
    cout << "+----+---------------------+\n";
    
    int pilihan;
    cout << "Masukkan nomor lokasi " << label << ": ";
    cin >> pilihan;
    
    // Validasi input
    while (pilihan < 1 || pilihan > lokasiList.size()) {
        cout << "Nomor tidak valid. Masukkan nomor antara 1-" 
             << lokasiList.size() << ": ";
        cin >> pilihan;
    }
    
    return pilihan - 1;
}

void simpanKeFile(Order& o) {
    // File untuk order aktif (selalu diperbarui)
    ofstream currentFile("order_active.txt");
    if (currentFile.is_open()) {
        currentFile << "====== Topsojek Surabaya ======\n";
        currentFile << "       ORDER ANDA AKTIF       \n";
        currentFile << "-------------------------------\n";
        currentFile << "Mohon tunggu driver di lokasi\n";
        currentFile << "-------------------------------\n";
        currentFile << "Nama Pelanggan: " << o.pelanggan << "\n";
        currentFile << "Driver: " << o.driver << "\n";
        currentFile << "Asal: " << o.asal << "\n";
        currentFile << "Tujuan: " << o.tujuan << "\n";
        currentFile << fixed << setprecision(2);
        currentFile << "Jarak: " << o.jarak << " km\n";
        currentFile << "Biaya: Rp" << o.biaya << "\n";
        currentFile << "Waktu: " << o.waktu << "\n";
        currentFile << "Status: " << (o.selesai ? "Selesai" : "Aktif") << "\n";
        currentFile.close();
    } else {
        cout << "Gagal menyimpan order aktif.\n";
    }

    // Hanya simpan ke history jika order SELESAI
    if (o.selesai) {
        ofstream historyFile("order_history.txt", ios::app);
        if (historyFile.is_open()) {
            historyFile << "\n======= Topsojek Surabaya =======\n";
            historyFile << "===      ORDER HISTORY      ===\n";
            historyFile << "-------------------------------\n";
            historyFile << "Tanggal: " << o.waktu << "\n";
            historyFile << "Nama Pelanggan: " << o.pelanggan << "\n";
            historyFile << "Driver: " << o.driver << "\n";
            historyFile << "Asal: " << o.asal << "\n";
            historyFile << "Tujuan: " << o.tujuan << "\n";
            historyFile << fixed << setprecision(2);
            historyFile << "Jarak: " << o.jarak << " km\n";
            historyFile << "Biaya: Rp" << o.biaya << "\n";
            historyFile << "Status: Selesai\n";
            historyFile << "-------------------------------\n";
            historyFile.close();
        } else {
            cout << "Gagal menyimpan history order.\n";
        }
    }
}

// CREATE
void createOrder() {
    // Cek jika ada order aktif yang belum selesai
    if (orderAktif != nullptr && !orderAktif->selesai) {
        cout << "\nOrder masih aktif. Tidak dapat membuat order baru.\n";
        cout << "Silakan selesaikan order terlebih dahulu.\n";
        getch();
        return;
    }

    // Jika ada order sebelumnya tapi sudah selesai, hapus dulu
    if (orderAktif != nullptr && orderAktif->selesai) {
        delete orderAktif;
        orderAktif = nullptr;
    }

    Order* o = new Order();
    cin.ignore();
    
    // Tampilkan header tanpa clear screen
    cout << "\n\n=== Tambah Order Baru ===";
    cout << "\n=========================";
    cout << "\nNama pelanggan: ";
    getline(cin, o->pelanggan);

    int asalIdx = pilihLokasi("Asal");
    int tujuanIdx = pilihLokasi("Tujuan");

    if (asalIdx < 0 || asalIdx >= lokasiList.size() || 
        tujuanIdx < 0 || tujuanIdx >= lokasiList.size()) {
        cout << "\nInput lokasi tidak valid!";
        delete o;
        getch();
        return;
    }

    // Set data order
    o->asal = lokasiList[asalIdx];
    o->tujuan = lokasiList[tujuanIdx];
    o->jarak = jarakMatrix[asalIdx][tujuanIdx];
    o->biaya = o->jarak * TARIF_PER_KM;
    o->driver = getRandomDriver();
    o->waktu = getWaktu();
    o->selesai = false;

    orderAktif = o;
    simpanKeFile(*orderAktif);

    // Tampilkan detail order tanpa clear screen
    cout << "\n\n=== ORDER BERHASIL DIBUAT ===";
    cout << "\n+--------------------------------------+\n";
    cout << "\n          Topsjek Surabaya         \n";
    cout << "Selamat Datang di Topsjek Lokal Surabaya\n";
    cout << " Telp. 08576667716, www.ojeklokal.com\n";
    cout << "\n----------------------------------------------";
    cout << "\n      ORDER ANDA AKTIF        ";
    cout << "\n-------------------------------";
    cout << "\nNama Pelanggan : " << o->pelanggan;
    cout << "\nDriver         : " << o->driver;
    cout << "\nAsal           : " << o->asal;
    cout << "\nTujuan         : " << o->tujuan;
    cout << fixed << setprecision(2);
    cout << "\nJarak          : " << o->jarak << " km";
    cout << "\nBiaya          : Rp" << o->biaya;
    cout << "\nWaktu Order    : " << o->waktu;
    cout << "\nStatus         : Dalam Proses";
    cout << "\n-------------------------------";
    cout << "\nMohon tunggu driver di lokasi";
    cout << "\n-------------------------------";
    cout << "\nData telah tersimpan ke sistem";
    
}

// READ
void readOrder() {
    if (orderAktif == nullptr) {
        cout << "\nBelum ada order aktif.\n";
        getch();
        return;
    }

    Order& o = *orderAktif;
    
    // Tampilan order aktif tanpa clear screen
    cout << "\n\n=== DATA ORDER AKTIF ===";  // Tambah newline di awal
    cout << "\n+--------------------------------------+\n";
    cout << "\n          Topsjek Surabaya         \n";
    cout << "Selamat Datang di Topsjek Lokal Surabaya\n";
    cout << " Telp. 08576667716, www.ojeklokal.com\n";
    cout << "\n----------------------------------------------";
    cout << "\nMohon untuk menunggu driver sampai lokasi asal";
    cout << "\n----------------------------------------------";
    cout << "\nNama Pelanggan : " << o.pelanggan;
    cout << "\nDriver         : " << o.driver;
    cout << "\nAsal           : " << o.asal;
    cout << "\nTujuan         : " << o.tujuan;
    cout << fixed << setprecision(2);
    cout << "\nJarak          : " << o.jarak << " km";
    cout << "\nBiaya          : Rp" << o.biaya;
    cout << "\nWaktu          : " << o.waktu;
    cout << "\n---------------------------------------------";
    
    // Tambahan status pesanan
    if (o.selesai) {
        cout << "\nSTATUS       : SELESAI";
        cout << "\n---------------------------------------------";
        cout << "\nTerima kasih telah menggunakan Topsojek!";
    } else {
        cout << "\nSTATUS       : DALAM PERJALANAN";
        cout << "\n---------------------------------------------";
        
        // Konfirmasi status pesanan
        char konfirmasi;
        cout << "\nApakah pesanan sudah sampai? (y/n): ";
        cin >> konfirmasi;
        
        if (tolower(konfirmasi) == 'y') {
            o.selesai = true;
            cout << "\nStatus pesanan diubah menjadi SELESAI";
            
            // Simpan perubahan ke file (tetap bekerja)
            simpanKeFile(o);
            
            cout << "\nKembali ke menu utama...";
        } else {
            cout << "\nPesanan masih dalam proses...";
            cout << "\nSilakan tunggu driver Anda";
        }
    }
    getch(); // Tunggu input user di akhir
}

// UPDATE
void updateOrder() {
    if (orderAktif == nullptr) {
        cout << "Tidak ada order untuk diupdate.\n";
        getch();
        return;
    }

    Order& o = *orderAktif;
    cin.ignore();
    cout << "\n=== Update Order ===\n";
    cout << "Update nama pelanggan: ";
    getline(cin, o.pelanggan);

    int asalIdx = pilihLokasi("Asal");
    int tujuanIdx = pilihLokasi("Tujuan");

    if (asalIdx < 0 || asalIdx >= lokasiList.size() || tujuanIdx < 0 || tujuanIdx >= lokasiList.size()) {
        cout << "Input lokasi tidak valid!\n";
        getch();
        return;
    }

    o.asal = lokasiList[asalIdx];
    o.tujuan = lokasiList[tujuanIdx];
    o.jarak = jarakMatrix[asalIdx][tujuanIdx];
    o.biaya = o.jarak * TARIF_PER_KM;
    o.driver = getRandomDriver();
    o.waktu = getWaktu();

    simpanKeFile(o);

    // Your requested output format
    cout << "\n\n=== ORDER BERHASIL DIUBAH ===";
    cout << "\n+--------------------------------------+\n";
    cout << "\n          Topsjek Surabaya         \n";
    cout << "Selamat Datang di Topsjek Lokal Surabaya\n";
    cout << " Telp. 08576667716, www.ojeklokal.com\n";
    cout << "\n----------------------------------------------";
    cout << "\n      ORDER ANDA AKTIF        ";
    cout << "\n-------------------------------";
    cout << "\nNama Pelanggan : " << o.pelanggan;
    cout << "\nDriver         : " << o.driver;
    cout << "\nAsal           : " << o.asal;
    cout << "\nTujuan         : " << o.tujuan;
    cout << fixed << setprecision(2);
    cout << "\nJarak          : " << o.jarak << " km";
    cout << "\nBiaya          : Rp" << o.biaya;
    cout << "\nWaktu Order    : " << o.waktu;
    cout << "\nStatus         : Dalam Proses";
    cout << "\n-------------------------------";
    cout << "\nMohon tunggu driver di lokasi";
    cout << "\n-------------------------------";
    cout << "\nData telah tersimpan ke sistem";
    cout << "\nOrder berhasil diperbarui dan disimpan ke file.\n";
    
    // getch();
}

// DELETE
void deleteOrder() {
    if (orderAktif == nullptr) {
        cout << "Tidak ada order untuk dihapus.\n";
        return;
    }

    delete orderAktif;
    orderAktif = nullptr;

    ofstream file("order.txt");
    file << "Tidak ada order aktif.\n";
    file.close();

    cout << "Order berhasil dihapus dan file diperbarui.\n";
}
void menuAfter() {
    int pilihan;
    do {
        cout << "\n          Topsjek Surabaya         \n";
        cout << "Selamat Datang di Topsjek Lokal Surabaya\n";
        cout << " Telp. 08576667716, www.ojeklokal.com\n";
        cout << "\n+--------------------------------------+\n";
        cout << "\n=== MENU ORDER AKTIF ===\n";
        cout << "\n+--------------------------------------+\n";
        cout << "1. Update Order saya\n";
        cout << "2. Pesan Topsjek Sekarang\n";
        cout << "3. Batalkan Order saya\n";
        cout << "4. Kembali ke Menu Utama\n";
        cout << "Pilih menu (1-4): ";
        cin >> pilihan;

        switch(pilihan) {
            case 1: 
                updateOrder();
                break;
            case 2: 
                readOrder(); 
                return;
                break;
            case 3: 
                deleteOrder();
                return; // Kembali ke menu utama setelah hapus
            case 4: 
                return; // Kembali ke menu utama
            default: 
                cout << "Pilihan tidak valid!\n";
        }
    } while (true);
}

//MAIN MENU
int main() {
    srand(time(0));
    int pilihan;
    bool orderAktif = false;

    do {
        cout << "\n          Topsjek Surabaya         \n";
        cout << "Selamat Datang di Topsjek Lokal Surabaya\n";
        cout << " Telp. 08576667716, www.ojeklokal.com\n";
        cout << "\n+--------------------------------------+\n";
        cout << "=====        MENU OJEK ONLINE      =====\n";
        cout << "+--------------------------------------+\n";
        cout << "1. Tambah Pesanan Baru\n";
        cout << "2. Pesan Topsjek Sekarang \n";
        cout << "3. Update Order saya\n";
        cout << "4. Batalkan Order saya\n";
        cout << "5. Keluar\n";
        cout << "Pilih menu (1-5): ";

        cin >> pilihan;

        switch (pilihan) {
            case 1: createOrder(); 
                orderAktif = true; // Set flag menjadi true setelah create
                if (orderAktif) menuAfter();
                break;
            case 2: readOrder(); break;
            case 3: if (orderAktif) {
                    updateOrder();
                    menuAfter();
                } else {
                    cout << "Tidak ada order aktif untuk diupdate!\n";
                }break;
            case 4: if (orderAktif) {
                    deleteOrder();
                    orderAktif = false; // Set flag menjadi false setelah delete
                } else {
                    cout << "Tidak ada order aktif untuk dihapus!\n";
                }break;
            case 5: cout << "Terima kasih Sudah Memesan Di Aplikasi Kami.\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }

    } while (pilihan != 5);

    // if (orderAktif != nullptr) delete orderAktif;

    return 0;
}