#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iomanip>

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
    for (int i = 0; i < lokasiList.size(); ++i) {
        cout << i + 1 << ". " << lokasiList[i] << endl;
    }
    int pilihan;
    cout << "Masukkan nomor lokasi " << label << ": ";
    cin >> pilihan;
    return pilihan - 1;
}

void simpanKeFile(Order& o) {
    ofstream file("order.txt");
    if (file.is_open()) {
        file << "======        Topsojek Surabaya         ======\n";
        file <<"            ORDER ANDA SEDANG AKTIF            \n";
        file << "----------------------------------------------\n";
        file << "Mohon untuk menunggu driver sampai lokasi asal\n";
        file << "----------------------------------------------\n";
        file << "Nama Pelanggan : " << o.pelanggan << "\n";
        file << "Driver         : " << o.driver << "\n";
        file << "Asal           : " << o.asal << "\n";
        file << "Tujuan         : " << o.tujuan << "\n";
        file << fixed << setprecision(2);
        file << "Jarak          : " << o.jarak << " km\n";
        file << "Biaya          : Rp" << o.biaya << "\n";
        file << "Waktu          : " << o.waktu;
        file.close();
    } else {
        cout << "Gagal menyimpan ke file.\n";
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
    cout << "Nama Pelanggan : " << o.pelanggan << endl;
    cout << "Driver         : " << o.driver << endl;
    cout << "Asal           : " << o.asal << endl;
    cout << "Tujuan         : " << o.tujuan << endl;
    cout << "Jarak          : " << o.jarak << " km\n";
    cout << fixed << setprecision(2);
    cout << "Biaya          : Rp" << o.biaya << endl;
    cout << "Waktu          : " << o.waktu;
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

// MAIN MENU
int main() {
    srand(time(0));
    int pilihan;

    do {
        cout << "\n          Topsjek Surabaya         \n";
        cout << "Selamat Datang di Topsjek Lokal Surabaya\n";
        cout << " Telp. 08576667716, www.ojeklokal.com\n";
        cout << "\n+--------------------------------------+\n";
        cout << "=====        MENU OJEK ONLINE      =====\n";
        cout << "+--------------------------------------+\n";
        cout << "1. Tambah Pesanan Baru\n";
        cout << "2. Lihat Orderan Aktif \n";
        cout << "3. Update Order saya\n";
        cout << "4. Hapus Order saya\n";
        cout << "5. Keluar\n";
        cout << "Pilih menu (1-5): ";

        cin >> pilihan;

        switch (pilihan) {
            case 1: createOrder(); break;
            case 2: readOrder(); break;
            case 3: updateOrder(); break;
            case 4: deleteOrder(); break;
            case 0: cout << "Terima kasih.\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }

    } while (pilihan != 0);

    if (orderAktif != nullptr) delete orderAktif;

    return 0;
}
