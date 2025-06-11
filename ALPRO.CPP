#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iomanip>

using namespace std;

const float TARIF_PER_KM = 5000.00;

// Daftar lokasi tetap
vector<string> lokasiList = {"Terminal", "Stasiun", "Pasar", "Kampus", "Mall"};

// Matriks jarak antar lokasi (dalam km)
int jarakMatrix[5][5] = {
    {0, 3, 4, 5, 6},
    {3, 0, 2, 4, 5},
    {4, 2, 0, 3, 4},
    {5, 4, 3, 0, 2},
    {6, 5, 4, 2, 0}
};

vector<string> driverList = {"Budi", "Siti", "Joko", "Dina", "Rizal"};

struct Order {
    string pelanggan;
    string driver;
    string asal;
    string tujuan;
    float jarak;
    float biaya;
    string waktu;
};

vector<Order> daftarOrder;

// Fungsi waktu sistem
string getWaktu() {
    time_t now = time(0);
    char* dt = ctime(&now);
    return string(dt);
}

// Fungsi ambil driver acak
string getRandomDriver() {
    return driverList[rand() % driverList.size()];
}

// Fungsi pilih lokasi
int pilihLokasi(string label) {
    cout << "\nPilih Lokasi " << label << ":\n";
    for (int i = 0; i < lokasiList.size(); ++i) {
        cout << i + 1 << ". " << lokasiList[i] << endl;
    }
    int pilihan;
    cout << "Masukkan nomor lokasi " << label << ": ";
    cin >> pilihan;
    return pilihan - 1;
}

// CREATE Order
void createOrder() {
    Order o;
    cin.ignore();
    cout << "\n=== Tambah Order ===\n";
    cout << "Nama pelanggan : ";
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
    o.driver = getRandomDriver();
    o.waktu = getWaktu();

    daftarOrder.push_back(o);
    cout << "Order berhasil ditambahkan.\n";
}

// READ Orders
void readOrders() {
    if (daftarOrder.empty()) {
        cout << "Belum ada data order.\n";
        return;
    }

    cout << "\n=== Data Order ===\n";
    for (int i = 0; i < daftarOrder.size(); ++i) {
        Order& o = daftarOrder[i];
        cout << "\nOrder ke-" << i + 1 << ":\n";
        cout << "Nama Pelanggan : " << o.pelanggan << endl;
        cout << "Driver         : " << o.driver << endl;
        cout << "Asal           : " << o.asal << endl;
        cout << "Tujuan         : " << o.tujuan << endl;
        cout << "Jarak          : " << o.jarak << " km\n";
        cout << fixed << setprecision(2);
        cout << "Biaya          : Rp" << o.biaya << endl;
        cout << "Waktu          : " << o.waktu;
    }
}

// UPDATE Order
void updateOrder() {
    if (daftarOrder.empty()) {
        cout << "Tidak ada data untuk diupdate.\n";
        return;
    }

    int idx;
    cout << "Masukkan nomor order yang ingin diupdate (1 - " << daftarOrder.size() << "): ";
    cin >> idx;

    if (idx < 1 || idx > daftarOrder.size()) {
        cout << "Nomor tidak valid.\n";
        return;
    }

    Order& o = daftarOrder[idx - 1];
    cin.ignore();
    cout << "Update nama pelanggan : ";
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
    o.driver = getRandomDriver(); // Ganti driver juga
    o.waktu = getWaktu();

    cout << "Order berhasil diperbarui.\n";
}

// DELETE Order
void deleteOrder() {
    if (daftarOrder.empty()) {
        cout << "Tidak ada data untuk dihapus.\n";
        return;
    }

    int idx;
    cout << "Masukkan nomor order yang ingin dihapus (1 - " << daftarOrder.size() << "): ";
    cin >> idx;

    if (idx < 1 || idx > daftarOrder.size()) {
        cout << "Nomor tidak valid.\n";
        return;
    }

    daftarOrder.erase(daftarOrder.begin() + (idx - 1));
    cout << "Order berhasil dihapus.\n";
}

// MAIN Program
int main() {
    srand(time(0));
    int pilihan;

    do {
        cout << "\n===== MENU OJEK ONLINE =====\n";
        cout << "==============================\n";
        cout << "Selamat Datang Pelanggan setia\n";
        cout << "==============================\n";
        cout << "\n1. Tambah Order (Create)\n";
        cout << "2. Lihat Order (Read)\n";
        cout << "3. Ubah Order (Update)\n";
        cout << "4. Hapus Order (Delete)\n";
        cout << "0. Keluar\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: createOrder(); break;
            case 2: readOrders(); break;
            case 3: updateOrder(); break;
            case 4: deleteOrder(); break;
            case 0: cout << "Terima kasih telah menggunakan Ojek Online.\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }

    } while (pilihan != 0);

    return 0;
}
