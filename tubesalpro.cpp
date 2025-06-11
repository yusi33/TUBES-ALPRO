#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <limits>

using namespace std;

// Struktur data untuk menyimpan informasi pesanan
struct Pesanan {
    int id;
    string namaPelanggan;
    string lokasiAwal;
    string lokasiTujuan;
    string namaDriver;
    int biaya;
    string waktuPemesanan;
};

// Database driver yang tersedia
vector<string> drivers = {
    "Budi Santoso", "Agus Wijaya", "Rina Permata", 
    "Dewi Kurnia", "Eko Pratama", "Fajar Nugroho"
};

// Database lokasi dan biaya
vector<pair<string, int>> lokasiBiaya = {
    {"Sudirman", 15000}, {"Thamrin", 20000}, {"Kuningan", 25000},
    {"Gatot Subroto", 18000}, {"Pondok Indah", 30000}, {"Kebayoran", 22000},
    {"Cilandak", 27000}, {"Mampang", 19000}, {"Tebet", 21000},
    {"Senayan", 23000}
};

vector<Pesanan> databasePesanan;
int nextId = 1;

// Fungsi untuk mendapatkan waktu saat ini
string getCurrentTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buffer);
}

// Fungsi untuk memilih driver secara acak
string getRandomDriver() {
    int index = rand() % drivers.size();
    return drivers[index];
}

// Fungsi untuk menghitung biaya berdasarkan lokasi
int hitungBiaya(const string& lokasiAwal, const string& lokasiTujuan) {
    int biayaAwal = 0, biayaTujuan = 0;
    
    for (const auto& loc : lokasiBiaya) {
        if (loc.first == lokasiAwal) biayaAwal = loc.second;
        if (loc.first == lokasiTujuan) biayaTujuan = loc.second;
    }
    
    // Biaya dasar + selisih biaya lokasi
    return 10000 + abs(biayaAwal - biayaTujuan);
}

// Fungsi untuk menampilkan semua lokasi yang tersedia
void tampilkanLokasiTersedia() {
    cout << "\nLokasi yang tersedia:\n";
    for (size_t i = 0; i < lokasiBiaya.size(); ++i) {
        cout << i+1 << ". " << lokasiBiaya[i].first << " (Rp" << lokasiBiaya[i].second << ")\n";
    }
}

// Fungsi untuk menambahkan pesanan baru
void tambahPesanan() {
    Pesanan pesananBaru;
    pesananBaru.id = nextId++;
    
    cout << "\n=== TAMBAH PESANAN BARU ===\n";
    
    // Input nama pelanggan
    cout << "Masukkan nama pelanggan: ";
    cin.ignore();
    getline(cin, pesananBaru.namaPelanggan);
    
    // Pilih lokasi awal
    tampilkanLokasiTersedia();
    cout << "Pilih lokasi awal (1-" << lokasiBiaya.size() << "): ";
    int pilihan;
    cin >> pilihan;
    while (pilihan < 1 || pilihan > static_cast<int>(lokasiBiaya.size())) {
        cout << "Pilihan tidak valid. Silakan pilih lagi: ";
        cin >> pilihan;
    }
    pesananBaru.lokasiAwal = lokasiBiaya[pilihan-1].first;
    
    // Pilih lokasi tujuan
    tampilkanLokasiTersedia();
    cout << "Pilih lokasi tujuan (1-" << lokasiBiaya.size() << "): ";
    cin >> pilihan;
    while (pilihan < 1 || pilihan > static_cast<int>(lokasiBiaya.size())) {
        cout << "Pilihan tidak valid. Silakan pilih lagi: ";
        cin >> pilihan;
    }
    pesananBaru.lokasiTujuan = lokasiBiaya[pilihan-1].first;
    
    // Generate data otomatis
    pesananBaru.namaDriver = getRandomDriver();
    pesananBaru.biaya = hitungBiaya(pesananBaru.lokasiAwal, pesananBaru.lokasiTujuan);
    pesananBaru.waktuPemesanan = getCurrentTime();
    
    // Tambahkan ke database
    databasePesanan.push_back(pesananBaru);
    
    cout << "\nPesanan berhasil ditambahkan!\n";
}

// Fungsi untuk menampilkan semua pesanan
void tampilkanSemuaPesanan() {
    if (databasePesanan.empty()) {
        cout << "\nBelum ada pesanan yang tercatat.\n";
        return;
    }
    
    cout << "\n=== DAFTAR SEMUA PESANAN ===\n";
    cout << "------------------------------------------------------------------------------------------\n";
    cout << left << setw(5) << "ID" << setw(20) << "Pelanggan" << setw(15) << "Lokasi Awal" 
         << setw(15) << "Lokasi Tujuan" << setw(20) << "Driver" 
         << setw(10) << "Biaya" << setw(20) << "Waktu Pemesanan" << endl;
    cout << "------------------------------------------------------------------------------------------\n";
    
    for (const auto& pesanan : databasePesanan) {
        cout << left << setw(5) << pesanan.id << setw(20) << pesanan.namaPelanggan 
             << setw(15) << pesanan.lokasiAwal << setw(15) << pesanan.lokasiTujuan 
             << setw(20) << pesanan.namaDriver << "Rp" << setw(8) << pesanan.biaya 
             << setw(20) << pesanan.waktuPemesanan << endl;
    }
    cout << "------------------------------------------------------------------------------------------\n";
}

// Fungsi untuk memperbarui pesanan (hanya nama pelanggan)
void updatePesanan() {
    if (databasePesanan.empty()) {
        cout << "\nBelum ada pesanan yang tercatat.\n";
        return;
    }
    
    tampilkanSemuaPesanan();
    
    int id;
    cout << "\nMasukkan ID pesanan yang ingin diupdate: ";
    cin >> id;
    
    bool found = false;
    for (auto& pesanan : databasePesanan) {
        if (pesanan.id == id) {
            found = true;
            cout << "\nData saat ini:\n";
            cout << "Pelanggan: " << pesanan.namaPelanggan << endl;
            cout << "Lokasi Awal: " << pesanan.lokasiAwal << endl;
            cout << "Lokasi Tujuan: " << pesanan.lokasiTujuan << endl;
            cout << "Driver: " << pesanan.namaDriver << endl;
            cout << "Biaya: Rp" << pesanan.biaya << endl;
            cout << "Waktu Pemesanan: " << pesanan.waktuPemesanan << endl;
            
            // Hanya bisa mengubah nama pelanggan
            cout << "\nMasukkan nama pelanggan yang baru: ";
            cin.ignore();
            getline(cin, pesanan.namaPelanggan);
            
            cout << "\nPesanan berhasil diupdate!\n";
            break;
        }
    }
    
    if (!found) {
        cout << "Pesanan dengan ID " << id << " tidak ditemukan.\n";
    }
}

// Fungsi untuk menghapus pesanan
void hapusPesanan() {
    if (databasePesanan.empty()) {
        cout << "\nBelum ada pesanan yang tercatat.\n";
        return;
    }
    
    tampilkanSemuaPesanan();
    
    int id;
    cout << "\nMasukkan ID pesanan yang ingin dihapus: ";
    cin >> id;
    
    for (auto it = databasePesanan.begin(); it != databasePesanan.end(); ++it) {
        if (it->id == id) {
            databasePesanan.erase(it);
            cout << "\nPesanan berhasil dihapus!\n";
            return;
        }
    }
    
    cout << "Pesanan dengan ID " << id << " tidak ditemukan.\n";
}

// Fungsi untuk menampilkan menu utama
void tampilkanMenu() {
    cout << "\n=== APLIKASI OJEK ONLINE ===\n";
    cout << "1. Tambah Pesanan Baru\n";
    cout << "2. Tampilkan Semua Pesanan\n";
    cout << "3. Update Pesanan\n";
    cout << "4. Hapus Pesanan\n";
    cout << "5. Keluar\n";
    cout << "Pilih menu (1-5): ";
}

int main() {
    srand(time(0)); // Seed untuk random number generator
    
    int pilihan;
    do {
        tampilkanMenu();
        cin >> pilihan;
        
        switch (pilihan) {
            case 1:
                tambahPesanan();
                break;
            case 2:
                tampilkanSemuaPesanan();
                break;
            case 3:
                updatePesanan();
                break;
            case 4:
                hapusPesanan();
                break;
            case 5:
                cout << "\nTerima kasih telah menggunakan aplikasi!\n";
                break;
            default:
                cout << "\nPilihan tidak valid. Silakan coba lagi.\n";
        }
        
    } while (pilihan != 5);
    
    return 0;
}