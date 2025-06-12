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
    // Mode append untuk menyimpan history
    ofstream file("order_history.txt", ios::app);
    
    if (file.is_open()) {
        // Header untuk setiap order baru
        file << "\n======= Topsojek Surabaya =======\n";
        file << "===      ORDER HISTORY      ===\n";
        file << "-------------------------------\n";
        file << "Tanggal: " << o.waktu << "\n";
        file << "Nama Pelanggan: " << o.pelanggan << "\n";
        file << "Driver: " << o.driver << "\n";
        file << "Asal: " << o.asal << "\n";
        file << "Tujuan: " << o.tujuan << "\n";
        file << fixed << setprecision(2);
        file << "Jarak: " << o.jarak << " km\n";
        file << "Biaya: Rp" << o.biaya << "\n";
        file << "Status: " << (o.selesai ? "Selesai" : "Aktif") << "\n";
        file << "-------------------------------\n";
        file.close();
    } else {
        cout << "Gagal menyimpan history order.\n";
    }

    // File terpisah untuk order aktif (akan di-overwrite)
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
        currentFile.close();
    } else {
        cout << "Gagal menyimpan order aktif.\n";
    }
}

// CREATE
void createOrder() {
    if (orderAktif != nullptr) {
        cout << "\nOrder sudah aktif. Tidak dapat membuat order baru.\n";
        return;
    }

    Order* o = new Order;
    cin.ignore();
    cout << "\n=== Tambah Order ===\n";
    cout << "Nama pelanggan: ";
    getline(cin, o->pelanggan);

    int asalIdx = pilihLokasi("Asal");
    int tujuanIdx = pilihLokasi("Tujuan");

    if (asalIdx < 0 || asalIdx >= lokasiList.size() || tujuanIdx < 0 || tujuanIdx >= lokasiList.size()) {
        cout << "Input lokasi tidak valid!\n";
        delete o;
        return;
    }

    o->asal = lokasiList[asalIdx];
    o->tujuan = lokasiList[tujuanIdx];
    o->jarak = jarakMatrix[asalIdx][tujuanIdx];
    o->biaya = o->jarak * TARIF_PER_KM;
    o->driver = getRandomDriver();
    o->waktu = getWaktu();

    orderAktif = o;
    simpanKeFile(*orderAktif);
    cout << "\n=== DATA ORDER AKTIF ===\n";
    cout << "======        Topsojek Surabaya         ======\n";
    cout << "            ORDER ANDA SEDANG AKTIF            \n";
    cout << "----------------------------------------------\n";
    cout << "Mohon untuk menunggu driver sampai lokasi asal\n";
    cout << "----------------------------------------------\n";
    cout << "Nama Pelanggan : " << o->pelanggan << endl;
    cout << "Driver         : " << o->driver << endl;
    cout << "Asal           : " << o->asal << endl;
    cout << "Tujuan         : " << o->tujuan << endl;
    cout << "Jarak          : " << o->jarak << " km\n";
    cout << fixed << setprecision(2);
    cout << "Biaya          : Rp" << o->biaya << endl;
    cout << "Waktu          : " << o->waktu << endl;
    cout << "Order berhasil dibuat dan disimpan ke file.\n";
}

// READ
void readOrder() {
    if (orderAktif == nullptr) {
        cout << "Belum ada order aktif.\n";
        return;
    }

    Order& o = *orderAktif;
    cout << "\n=== DATA ORDER AKTIF ===\n";
    cout << "======        Topsojek Surabaya         ======\n";
    cout <<"            ORDER ANDA SEDANG AKTIF            \n";
    cout << "----------------------------------------------\n";
    cout << "Mohon untuk menunggu driver sampai lokasi asal\n";
    cout << "----------------------------------------------\n";
    cout << "Nama Pelanggan : " << o.pelanggan << endl;
    cout << "Driver         : " << o.driver << endl;
    cout << "Asal           : " << o.asal << endl;
    cout << "Tujuan         : " << o.tujuan << endl;
    cout << "Jarak          : " << o.jarak << " km\n";
    cout << fixed << setprecision(2);
    cout << "Biaya          : Rp" << o.biaya << endl;
    cout << "Waktu          : " << o.waktu;
    cout << "/n---------------------------------------------\n";
    cout << "Terima kasih Sudah Memesan Di Aplikasi Kami\n";
    getch();
    
}

// UPDATE
void updateOrder() {
    if (orderAktif == nullptr) {
        cout << "Tidak ada order untuk diupdate.\n";
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
        return;
    }

    o.asal = lokasiList[asalIdx];
    o.tujuan = lokasiList[tujuanIdx];
    o.jarak = jarakMatrix[asalIdx][tujuanIdx];
    o.biaya = o.jarak * TARIF_PER_KM;
    o.driver = getRandomDriver(); // Driver bisa berubah
    o.waktu = getWaktu();

    simpanKeFile(o);
    cout << "Order berhasil diperbarui dan disimpan ke file.\n";
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
        cout << "1. Update Order\n";
        cout << "2. Pesan Topsjek Sekarang\n";
        cout << "3. Batalkan Order\n";
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
        cout << "4. Hapus Order saya\n";
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