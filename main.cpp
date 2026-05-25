#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <limits> 
#include <cctype>
#include <ctime>
using namespace std;

// projek uts: sistem manajemen stok barang minimarket
// materi yang harus ada: variable, tipe data, array, structure, pointer, single linked list, double linked list, 

void bersihkan_layar() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}


// =========================
// Ini adalah admin Gudang
// ========================

struct Barang {
    string kode_barang;
    string nama;
    string kategori;
    double harga_beli;
    double harga_jual;
    int stok;
    string satuan;
    string tanggal_kadaluarsa;
    string supplier;
};

struct LogBarang {
    string waktu;
    string aksi;
    string detail;
    LogBarang* next;
    LogBarang* prev;
};

LogBarang* logHead = NULL;
LogBarang* logTail = NULL;

void tambah_log(string aksi, string detail) {
    LogBarang* baru = new LogBarang;
    
    time_t now = time(0);
    char* dt = ctime(&now);
    string str_waktu(dt);
    if (!str_waktu.empty() && str_waktu[str_waktu.length()-1] == '\n') {
        str_waktu.erase(str_waktu.length()-1);
    }

    baru->waktu = str_waktu;
    baru->aksi = aksi;
    baru->detail = detail;
    baru->next = NULL;
    baru->prev = NULL;

    if (logHead == NULL) {
        logHead = logTail = baru;
    } else {
        logTail->next = baru;
        baru->prev = logTail;
        logTail = baru;
    }
}

void tampilkan_log_barang() {
    bersihkan_layar();
    if (logHead == NULL) {
        cout << "\n[ Riwayat log masih kosong ]" << endl;
        return;
    }

    cout << "\n" << string(100, '=') << endl;
    cout << "                             RIWAYAT LOG AKTIVITAS (LIFO)                             " << endl;
    cout << string(100, '=') << endl;
    cout << left << setw(25) << "Waktu" << setw(20) << "Aksi" << "Detail" << endl;
    cout << string(100, '-') << endl;

    LogBarang* current = logTail;
    while (current != NULL) {
        cout << left << setw(25) << current->waktu 
             << setw(20) << current->aksi 
             << current->detail << endl;
        current = current->prev;
    }
    cout << string(100, '=') << endl;
}

struct NodeBarang {
	Barang data;
	NodeBarang* next;
};

struct KategoriNode {
	string nama;
	string kode_kat;
	int level;
	KategoriNode* first_child;
	KategoriNode* next_sibling;
	KategoriNode* parent;
	
	KategoriNode(string nama, int lvl = 0) {
        this->nama = nama;
        this->level = lvl;
        this->first_child = NULL;
        this->next_sibling = NULL;
        this->parent = NULL;
        
//        static int counter = 1;
//        char buffer[20];
//        sprintf(buffer, "KAT%03d", counter++);
////        this->kode_kategori = string(buffer);
    }
};

KategoriNode* rootKategori = NULL;
NodeBarang* head = NULL;
NodeBarang* tail = NULL;

void tambah_barang();
void tampilkan_barang();
bool is_kosong();
string generate_kode_barang();
void register_user();
//void edit_barang();

Barang br;


bool is_kosong() {
    return head == NULL;
}

string generate_kode_barang() {
    static int nomor = 1;

    char buffer[20];
    sprintf(buffer, "BRG%03d", nomor++);

    return string(buffer);
}

string toLowerCase(string text) {
    for (int i = 0; i < text.length(); i++) {
        text[i] = tolower(text[i]);
    }
    return text;
}

bool is_valid_date_format(const string& date) {
    if (date.length() != 10) return false;
    if (date[2] != '-' || date[5] != '-') return false;

    for (int i = 0; i < date.length(); i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(date[i])) return false;
    }

    int d = atoi(date.substr(0,2).c_str());
    int m = atoi(date.substr(3,2).c_str());
    int y = atoi(date.substr(6,4).c_str());
    
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > 31) return false;

    if (m == 4 || m == 6 || m == 9 || m == 11) {
        if (d > 30) return false;
    }

    if (m == 2) {
        if (d > 29) return false;
    }

    return true;
}

bool is_future_date(const string& date) {
    int d = atoi(date.substr(0,2).c_str());
	int m = atoi(date.substr(3,2).c_str());
	int y = atoi(date.substr(6,4).c_str());

    time_t t = time(0);
    tm* now = localtime(&t);

    int curr_d = now->tm_mday;
    int curr_m = now->tm_mon + 1;
    int curr_y = now->tm_year + 1900;

    if (y < curr_y) return false;
    if (y == curr_y && m < curr_m) return false;
    if (y == curr_y && m == curr_m && d < curr_d) return false;

    return true;
}

void tambah_barang() {
    bersihkan_layar();
    int jumlah;
    
    cout << "Jumlah barang yang ingin di-input: ";
    cin >> jumlah;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "\n=== TAMBAH BARANG ===\n";
    
    for (int i = 0; i < jumlah; i++) {
        cout << "\nMasukkan data ke-" << (i+1) << ": \n";
        
        br.kode_barang = generate_kode_barang();
        
        cout << "Kode barang (otomatis): " << br.kode_barang << endl;
        
        cout << "> Nama barang: "; 
		getline(cin, br.nama);
		while(br.nama.empty()) {
    		cout << "Nama tidak boleh kosong! Masukkan lagi: ";
    		getline(cin, br.nama);
		}
        
        cout << "> Kategori [Enter jika lainnya]: "; 
        getline(cin, br.kategori);
        if(br.kategori.empty()) br.kategori = "Lainnya";
        
        string input;
		while (true) {
    		cout << "> Harga beli: ";
    		getline(cin, input);

    		if (input.empty()) {
        		cout << "Input tidak boleh kosong! Masukkan lagi: \n";
        		continue;
    		}

    		bool valid = true;
    		for (int i = 0; i < input.length(); i++) {
        		if (!isdigit(input[i]) && input[i] != '.') {
            		valid = false;
            		break;
        		}	
    		}

    		if (!valid) {
        		cout << "Input harus angka! Masukkan lagi: \n";
        		continue;
    		}

    		br.harga_beli = atof(input.c_str());
    		break;
		}
		
		while (true) {
    		cout << "> Harga jual: ";
    		getline(cin, input);

    		if (input.empty()) {
        		cout << "Input tidak boleh kosong! Masukkan lagi: \n";
        		continue;
    		}

    		bool valid = true;
    		for (int i = 0; i < input.length(); i++) {
        		if (!isdigit(input[i]) && input[i] != '.') {
            		valid = false;
            		break;
        		}	
    		}

    		if (!valid) {
        		cout << "Input harus angka! Masukkan lagi: \n";
        		continue;
    		}

    		br.harga_jual = atof(input.c_str());
    		break;
		}
        
        while (true) {
    		cout << "> Stok awal: ";
    		getline(cin, input);

		    if (input.empty()) {
		        cout << "Input tidak boleh kosong! Masukkan lagi: \n";
		        continue;
    		}

    		bool valid = true;
    		for (int i = 0; i < input.length(); i++) {
        		if (!isdigit(input[i])) {
            		valid = false;
            		break;
        		}
    		}	

    		if (!valid) {
        		cout << "Input harus angka! Masukkan lagi: \n";
        		continue;
    		}

    		br.stok = atoi(input.c_str());
    		break;
		}
                
        cout << "> Satuan [Enter jika pcs]: "; 
        getline(cin, br.satuan);
        if(br.satuan.empty()) br.satuan = "pcs";
        
        cout << "> Tanggal kadaluarsa (DD-MM-YYYY) [Enter jika tidak ada]: ";
		getline(cin, br.tanggal_kadaluarsa);

		if(br.tanggal_kadaluarsa.empty()) {
    		br.tanggal_kadaluarsa = "-";
		} else {
    		while(true) {
        		if(!is_valid_date_format(br.tanggal_kadaluarsa)) {
            		cout << "Format tanggal salah! Gunakan DD-MM-YYYY: ";
        		} else if(!is_future_date(br.tanggal_kadaluarsa)) {
            		cout << "Tanggal tidak boleh kurang dari hari ini! Masukkan lagi: ";
        		} else {
            		break;
        		}
        		getline(cin, br.tanggal_kadaluarsa);
    		}
		}
        
        cout << "> Supplier [Enter jika tidak ada]: "; 
        getline(cin, br.supplier);
        if(br.supplier.empty()) br.supplier = "-";
        
        NodeBarang* new_node = new NodeBarang();
        new_node->data = br;
        new_node->next = NULL;
        
        if(is_kosong()) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }

        tambah_log("Tambah Barang", "Kode: " + new_node->data.kode_barang + " | Nama: " + new_node->data.nama);
        
        cout << "\n[Barang ke-" << (i+1) << " berhasil ditambahkan!]" << endl;
        cout << "\n" << string(100, '-') << endl;
    }
}

void tampilkan_barang() {
    bersihkan_layar();
    if (is_kosong()) {
        cout << "List Barang kosong." << endl;
        return;
    }

    cout << "\n=== DAFTAR BARANG ===" << endl;
    cout << string(148, '=') << endl;

    cout << left << setw(5)  << "No"
         << setw(12) << "Kode"
         << setw(20) << "Nama"
         << setw(15) << "Kategori"
         << setw(12) << "Hrg Beli"
         << setw(12) << "Hrg Jual"
         << setw(8)  << "Stok"
         << setw(10) << "Satuan"
         << setw(15) << "Expired"
         << "Supplier" << endl;

    cout << string(148, '-') << endl;

    NodeBarang* current = head;
    int no = 1;

    while (current != NULL) {
        cout << left << setw(5)  << no++
             << setw(12) << current->data.kode_barang
             << setw(20) << current->data.nama
             << setw(15) << current->data.kategori
             << setw(12) << fixed << setprecision(0) << current->data.harga_beli
             << setw(12) << current->data.harga_jual
             << setw(8)  << current->data.stok
             << setw(10) << current->data.satuan
             << setw(15) << current->data.tanggal_kadaluarsa
             << current->data.supplier << endl;
        current = current->next; 
    }
    
    cout << string(148, '=') << endl;
}

// Fitur Mengahapus barang 
void hapus_barang() {
    bersihkan_layar();
    if (is_kosong()) {
        cout << "List barang kosong.\n";
        return;
    }

    string kode;
    char konfirmasi;

    cout << "\nMasukkan kode barang yang ingin dihapus: ";
    cin >> kode;

    NodeBarang* current = head;
    NodeBarang* prev = NULL;

   while (current != NULL &&
       toLowerCase(current->data.kode_barang) != toLowerCase(kode)) {

    prev = current;
    current = current->next;
}
    if (current == NULL) {
        cout << "Barang dengan kode " << kode << " tidak ditemukan.\n";
        return;
    }

    // tampilkan barang yang akan dihapus
    cout << "\nData yang akan dihapus:\n";
    cout << "Kode       : " << current->data.kode_barang << endl;
    cout << "Nama       : " << current->data.nama << endl;
    cout << "Stok       : " << current->data.stok << endl;
    cout << "Harga      : Rp" << current->data.harga_jual << endl;

    cout << "\nYakin ingin menghapus? (Y/n): ";
    cin >> konfirmasi;

    if (konfirmasi == 'Y' || konfirmasi == 'y') {
        if (current == head) {
            head = head->next;
            if (head == NULL) {
                tail = NULL;
            }
        } 
        else {
            prev->next = current->next;

            if (current == tail) {
                tail = prev;
            }
        }

        delete current;
        tambah_log("Hapus Barang", "Kode: " + kode + " berhasil dihapus");
        cout << "Barang berhasil dihapus.\n";
    } 
    if(konfirmasi == 'n' || konfirmasi == 'N') {
        cout << "Penghapusan dibatalkan.\n";
    }
    else{
        cout <<"tidak valid hanya boleh y/n\n";
    }
}
void cari_barang() {
    bersihkan_layar();
    if (is_kosong()) {
        cout << "List barang kosong.\n";
        return;
    }

    int opsi;
    cout << "\n=== CARI BARANG ===\n";
    cout << "1. Berdasarkan Kode Barang\n";
    cout << "2. Berdasarkan Nama Barang\n";
    cout << "Pilih opsi: ";
    cin >> opsi;
    cin.ignore();

    NodeBarang* current = head;

   if (opsi == 1) {

    string kode;

    cout << "\nMasukkan kode barang yang ingin dicari: ";
    getline(cin, kode);

    while (current != NULL) {

        if (toLowerCase(current->data.kode_barang)
            == toLowerCase(kode)) {
            break;
        }

        current = current->next;
    }
}
    else if (opsi == 2) {
        string cari;
        cout << "\nMasukkan nama barang yang ingin dicari: ";
        getline(cin, cari);

        while (current != NULL &&
               toLowerCase(current->data.nama).find(toLowerCase(cari)) == string::npos) {
            current = current->next;
        }
    }
    else {
        cout << "Pilihan tidak valid! Gunakan 1 atau 2.\n";
        return;
    }

    if (current == NULL) {
        cout << "\nBarang tidak ditemukan.\n";
        return;
    }


    cout << "\n=== HASIL PENCARIAN ===\n";
    cout << string(60, '=') << endl;

    cout << "Kode               : " << current->data.kode_barang << endl;
    cout << "Nama               : " << current->data.nama << endl;
    cout << "Stok               : " << current->data.stok << endl;
    cout << "Kategori           : " << current->data.kategori << endl;
    cout << "Supplier           : " << current->data.supplier << endl;
    cout << "Tgl Kadaluarsa     : " << current->data.tanggal_kadaluarsa << endl;
    cout << "Harga              : Rp" << current->data.harga_jual << endl;
    cout << "Satuan             : " << current->data.satuan << endl;

    cout << string(60, '=') << endl;
}

// Fitur Update Barang
void update_barang(){
    bersihkan_layar();
    if (is_kosong() == 1){
        cout << "\nTidak ada barang yang dapat diupdate!\n";
    }
    else{
        tampilkan_barang();
        string kode;
        NodeBarang *current;

        while (true){
            cout << "\nMasukkan kode barang: ";
            cin >> kode;

            current = head;
            while (current != NULL){
                if (current->data.kode_barang == kode){
                    break;
                }
                current = current->next;
            }

            if (current != NULL){
                break;
            }
            else{
                cout << "\nKode barang tidak ditemukan, silakan input kembali!\n";
            }
        }

        int pilih;

        while (true){
            cout << "\n1. Update Nama";
            cout << "\n2. Update Kategori";
            cout << "\n3. Update Harga";
            cout << "\n4. Update Stok";
            cout << "\nPilih        : ";
            cin >> pilih;

            if (pilih >= 1 && pilih <= 4){
                break;
            }
            else{
                cout << "\nPilihan tidak valid, silakan input kembali!\n l";
            }
        }

        if (pilih == 1){
            string nama;
            cin.ignore();

            while (true){
                cout << "Nama baru      : ";
                getline(cin, nama);

                if (nama != ""){
                    current->data.nama = nama;
                    break;
                }
                else{
                    cout << "\nNama tidak boleh kosong, silakan input kembali!\n";
                }
            }
        }
        else if (pilih == 2){
            string kategori;
            cin.ignore();

            while (true){
                cout << "Kategori baru  : ";
                getline(cin, kategori);

                if (kategori != ""){
                    current->data.kategori = kategori;
                    break;
                }
                else{
                    cout << "\nKategori tidak boleh kosong, silakan input kembali!\n";
                }
            }
        }
        else if (pilih == 3){
            double harga;

            while (true){
                cout << "Harga baru     : ";
                cin >> harga;

                if (harga > 0){
                    current->data.harga_jual = harga;
                    break;
                }
                else{
                    cout << "\nHarga harus lebih dari 0, silakan input kembali!\n";
                }
            }
        }
        else if (pilih == 4){
            int jumlah;

            while (true){
                cout << "Stok sekarang: " << current->data.stok << endl;
                cout << "Tambah/Kurang stok (+/-): ";
                cin >> jumlah;

                if (current->data.stok + jumlah < 0){
                    cout << "\nStok tidak boleh minus, silakan input kembali!\n";
                }
                else{
                    current->data.stok = current->data.stok + jumlah;
                    break;
                }
            }
        }

        cout << "\nData berhasil diupdate!\n";
        tambah_log("Update Barang", "Update pada kode: " + kode);
    }
}
// ===============================
// ini adalah login dan register
// ===============================
struct User {
    string username;
    string password;
    string role;
};

User users[100] = {
    {"gudang", "123", "Admin Gudang"},
    {"kasir", "123", "Kasir"},
    {"customer", "123", "Customer"}
};

int jumlahUser = 3;

string login() {

    while (true) {

        bersihkan_layar();

        int pilih;

        cout << "=====================================\n";
        cout << "        SISTEM MINIMARKET\n";
        cout << "=====================================\n";

        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "0. Keluar\n";

        cout << "\nPilih menu : ";
        cin >> pilih;
        cin.ignore();

        if (pilih == 0) {
            return "Keluar";
        }

        else if (pilih == 2) {

            register_user();

            cout << "\nTekan ENTER untuk lanjut...";
            cin.get();

            continue;
        }

        else if (pilih == 1) {

            string username, password;

            cout << "\nUsername : ";
            getline(cin, username);

            cout << "Password : ";
            getline(cin, password);

            for (int i = 0; i < jumlahUser; i++) {

                if (username == users[i].username &&
                    password == users[i].password) {

                    cout << "\nLogin berhasil sebagai "
                         << users[i].role << "!\n";

                    return users[i].role;
                }
            }

            cout << "\nUsername atau password salah!\n";

            cout << "Tekan ENTER untuk coba lagi...";
            cin.get();
        }

        else {
            cout << "\nMenu tidak valid!\n";
            cin.get();
        }
    }
}

void register_user() {

    bersihkan_layar();

    string username, password;
    int role;

    cout << "=====================================\n";
    cout << "             REGISTER\n";
    cout << "=====================================\n";

    cout << "Username : ";
    getline(cin, username);

    // cek username sudah ada atau belum
    for (int i = 0; i < jumlahUser; i++) {

        if (users[i].username == username) {
            cout << "\nUsername sudah digunakan!\n";
            return;
        }
    }

    cout << "Password : ";
    getline(cin, password);

    cout << "\nPilih Role\n";
    cout << "1. Admin Gudang\n";
    cout << "2. Kasir\n";
    cout << "3. Customer\n";
    cout << "Pilih : ";
    cin >> role;
    cin.ignore();

    string roleUser;

    if (role == 1) {
        roleUser = "Admin Gudang";
    }
    else if (role == 2) {
        roleUser = "Kasir";
    }
    else if (role == 3) {
        roleUser = "Customer";
    }
    else {
        cout << "\nRole tidak valid!\n";
        return;
    }

    users[jumlahUser].username = username;
    users[jumlahUser].password = password;
    users[jumlahUser].role = roleUser;

    jumlahUser++;

    cout << "\nRegister berhasil!\n";
}

// ==============================
// ini adalah fungsi admin kasir
// ==============================

//  Kasir men input data untuk costumer yang melakukan transaksi secara OFFLINE
// queue dan STackk
struct QueueCustomer {
    string nama_customer;
    QueueCustomer* next;
};

QueueCustomer* frontQueue = NULL;
QueueCustomer* rearQueue = NULL;
void enqueueCustomer(string nama) {

    QueueCustomer* baru = new QueueCustomer();

    baru->nama_customer = nama;
    baru->next = NULL;

    if (frontQueue == NULL) {
        frontQueue = rearQueue = baru;
    }
    else {
        rearQueue->next = baru;
        rearQueue = baru;
    }
}
void dequeueCustomer() {

    if (frontQueue == NULL) {
        return;
    }

    QueueCustomer* hapus = frontQueue;

    frontQueue = frontQueue->next;

    if (frontQueue == NULL) {
        rearQueue = NULL;
    }

    delete hapus;
}
struct StackTransaksi {
    string receipt;
    StackTransaksi* next;
};
StackTransaksi* topTransaksi = NULL;
void pushTransaksi(string receipt) {

    StackTransaksi* baru = new StackTransaksi();

    baru->receipt = receipt;
    baru->next = topTransaksi;

    topTransaksi = baru;
}
void tampilkan_riwayat_transaksi() {

    bersihkan_layar();

    if (topTransaksi == NULL) {
        cout << "Belum ada transaksi.\n";
        return;
    }

    StackTransaksi* current = topTransaksi;

    cout << "\n=== RIWAYAT TRANSAKSI ===\n";

    while (current != NULL) {

        cout << current->receipt << endl;
        cout << "\n====================================\n";

        current = current->next;
    }
}
void transaksi_kasir() {

    bersihkan_layar();

    string namaCustomer;
    string kodeBarang;
    int jumlah;

    cout << "\n=========== TRANSAKSI KASIR ===========\n";

    cout << "Nama customer : ";
    getline(cin, namaCustomer);

    // masuk queue
    enqueueCustomer(namaCustomer);

    tampilkan_barang();

    cout << "\nMasukkan kode barang : ";
    getline(cin, kodeBarang);

    NodeBarang* current = head;

    while (current != NULL) {

        if (toLowerCase(current->data.kode_barang)
            == toLowerCase(kodeBarang)) {

            break;
        }

        current = current->next;
    }

    if (current == NULL) {

        cout << "\nBarang tidak ditemukan!\n";

        dequeueCustomer();
        return;
    }

    cout << "Jumlah beli : ";
    cin >> jumlah;
    cin.ignore();

    if (jumlah > current->data.stok) {

        cout << "\nStok tidak cukup!\n";

        dequeueCustomer();
        return;
    }

    // stok otomatis berkurang
    current->data.stok -= jumlah;

    double total = jumlah * current->data.harga_jual;

    // receipt
    string receipt =
        "\n=========== RECEIPT ===========\n"
        "Nama Customer : " + namaCustomer +
        "\nBarang         : " + current->data.nama +
        "\nJumlah         : " + to_string(jumlah) +
        "\nHarga          : Rp" + to_string((int)current->data.harga_jual) +
        "\nTotal          : Rp" + to_string((int)total) +
        "\n================================";

    cout << receipt << endl;

    // masuk stack transaksi
    pushTransaksi(receipt);

    // log
    tambah_log(
        "Transaksi",
        namaCustomer + " membeli " + current->data.nama
    );

    // keluar queue
    dequeueCustomer();
}
// =========================================
// ini adalah fungsi customer (online)
// =========================================

// 1. tambahin barang ke keranjang (customer online)
// Pake single linked list
struct Keranjang {
    string nama_barang;
    int jumlah;
    double harga;
    Keranjang* next;
};

Keranjang* headKeranjang = NULL;
Keranjang* tailKeranjang = NULL;

struct RiwayatCustomer {
    string nama_customer;
    string barang;
    int jumlah;
    double total;
    string status;
    RiwayatCustomer* next;
};

RiwayatCustomer* headRiwayat = NULL;
RiwayatCustomer* tailRiwayat = NULL;

void tambah_riwayat_customer(string nama, string barang, int jumlah, double total, string status) {

    RiwayatCustomer* baru = new RiwayatCustomer();

    baru->nama_customer = nama;
    baru->barang = barang;
    baru->jumlah = jumlah;
    baru->total = total;
    baru->status = status;
    baru->next = NULL;

    if (headRiwayat == NULL) {
        headRiwayat = tailRiwayat = baru;
    }
    else {
        tailRiwayat->next = baru;
        tailRiwayat = baru;
    }
}


void tampilkan_riwayat_customer(string namaCustomer) {

    bersihkan_layar();

    if (headRiwayat == NULL) {
        cout << "Belum ada riwayat pembelian.\n";
        return;
    }

    RiwayatCustomer* current = headRiwayat;

    bool ditemukan = false;
    int no = 1;

    cout << "\n======= RIWAYAT PEMBELIAN =======\n";

    while (current != NULL) {

        if (toLowerCase(current->nama_customer)
            == toLowerCase(namaCustomer)) {

            cout << no++ << ". "
                 << current->barang
                 << " | Jumlah: " << current->jumlah
                 << " | Total: Rp" << current->total
                 << " | Status: " << current->status
                 << endl;

            ditemukan = true;
        }

        current = current->next;
    }

    if (!ditemukan) {
        cout << "\nBelum ada transaksi.\n";
    }
}


void tambah_ke_keranjang() {

    bersihkan_layar();

    if (is_kosong()) {
        cout << "Barang masih kosong.\n";
        return;
    }

    tampilkan_barang();

    string nama;
    int jumlah;

    cout << "\nMasukkan Nama barang: ";
    getline(cin, nama);

    NodeBarang* current = head;

    while (current != NULL) {

        if (toLowerCase(current->data.nama)
            == toLowerCase(nama)){
            break;
        }

        current = current->next;
    }

    if (current == NULL) {
        cout << "\nBarang tidak ditemukan!\n";
        return;
    }

    cout << "Jumlah beli: ";
    cin >> jumlah;
    cin.ignore();

    if (jumlah <= 0) {
        cout << "\nJumlah tidak valid!\n";
        return;
    }

    if (jumlah > current->data.stok) {
        cout << "\nStok tidak cukup!\n";
        return;
    }

  
    Keranjang* baru = new Keranjang();

    baru->nama_barang = current->data.nama;
    baru->jumlah = jumlah;
    baru->harga = current->data.harga_jual;
    baru->next = NULL;

    if (headKeranjang == NULL) {
        headKeranjang = tailKeranjang = baru;
    }
    else {
        tailKeranjang->next = baru;
        tailKeranjang = baru;
    }

    cout << "\nBarang berhasil ditambahkan ke keranjang!\n";
   
    double total = jumlah * current->data.harga_jual;

    tambah_riwayat_customer(
        "customer",
        current->data.nama,
        jumlah,
        total,
        "Diproses"
    );
}
void tampilkan_keranjang() {

    bersihkan_layar();

    if (headKeranjang == NULL) {
        cout << "Keranjang masih kosong.\n";
        return;
    }

    Keranjang* current = headKeranjang;

    int no = 1;
    double grandTotal = 0;

    cout << "\n========== KERANJANG ==========\n";

    while (current != NULL) {

        double subtotal = current->jumlah * current->harga;

        cout << no++ << ". "
             << current->nama_barang
             << " | Jumlah: " << current->jumlah
             << " | Harga: Rp" << current->harga
             << " | Subtotal: Rp" << subtotal
             << endl;

        grandTotal += subtotal;

        current = current->next;
    }

    cout << "================================\n";
    cout << "Total Belanja : Rp" << grandTotal << endl;
}

void hapus_keranjang(){

    bersihkan_layar();

    if (headKeranjang == NULL){
        cout << "Keranjang masih kosong!\n";
        return;
    }

    tampilkan_keranjang();

    string nama;

    cout << "\nBarang yang ingin dihapus: ";
    getline(cin, nama);

    Keranjang* current = headKeranjang;
    Keranjang* prev = NULL;

    while (current != NULL){

        if (toLowerCase(current->nama_barang)
            == toLowerCase(nama)){
            break;
        }

        prev = current;
        current = current->next;
    }

    // tidak ditemukan
    if (current == NULL){
        cout << "\nBarang tidak ditemukan di keranjang!\n";
        return;
    }

    // ditemukan
    if (current == headKeranjang) {
        headKeranjang = headKeranjang->next;

        if (headKeranjang == NULL) {
            tailKeranjang = NULL;
        }
    }
    else {
        prev->next = current->next;

        if (current == tailKeranjang) {
            tailKeranjang = prev;
        }
    }

    delete current;

    cout << "\nBarang berhasil dihapus dari keranjang!\n";
}

// 2. cari barang berdasarkan nama untuk customer
void cari_barang_customer() {

    bersihkan_layar();

    if (is_kosong()) {
        cout << "Barang masih kosong.\n";
        return;
    }

    string keyword;

    cout << "\n=========== CARI BARANG ===========\n";
    cout << "Masukkan nama barang : ";
    getline(cin, keyword);

    NodeBarang* current = head;

    bool ditemukan = false;
    int no = 1;

    cout << "\n=========== HASIL PENCARIAN ===========\n";

    while (current != NULL) {

        if (toLowerCase(current->data.nama)
            .find(toLowerCase(keyword)) != string::npos) {

            cout << "\nBarang ke-" << no++ << endl;

            cout << "Kode Barang : "
                 << current->data.kode_barang << endl;

            cout << "Nama        : "
                 << current->data.nama << endl;

            cout << "Kategori    : "
                 << current->data.kategori << endl;

            cout << "Harga       : Rp"
                 << current->data.harga_jual << endl;

            cout << "Stok        : "
                 << current->data.stok << endl;

            cout << "Supplier    : "
                 << current->data.supplier << endl;

            cout << "----------------------------------\n";

            ditemukan = true;
        }

        current = current->next;
    }

    if (!ditemukan) {
        cout << "\nBarang tidak ditemukan.\n";
    }
}

void init_kat() {
//	root_kat = new KategoriNode("SEMUA BARANG", 0);
	
	// lvl 1
	KategoriNode* makanan_minuman = new KategoriNode("Makanan & Minuman", 1);
	KategoriNode* rumah_tangga = new KategoriNode("Kebutuhan Rumah Tangga", 1);
	KategoriNode* perawatan = new KategoriNode("Perawatan Diri & Kesehatan", 1);
	KategoriNode* alat_tulis = new KategoriNode("Alat Tulis & Kantor", 1);
	KategoriNode* lainnya = new KategoriNode("Lainnya", 1);
	
	root_kat->first_child = makanan_minuman;
	makanan_minuman->next_sibling = rumah_tangga;
	rumah_tangga->next_sibling = perawatan;
	perawatan->next_sibling = alat_tulis;
	alat_tulis->next_sibling = lainnya;
	
	makanan_minuman->parent = root_kat;
	rumah_tangga->parent = root_kat;
	perawatan->parent = root_kat;
	alat_tulis->parent = root_kat;
	lainnya->parent = root_kat;
}


int main() {

    while (true) {

        string roleUser = login();

        // pilihan keluar dari login
        if (roleUser == "Keluar") {
            cout << "\nProgram selesai.\n";
            break;
        }
        
        // ================= ADMIN GUDANG =================
        if (roleUser == "Admin Gudang") {

            int pilihanGudang;

            while (true) {

                bersihkan_layar();

                cout << "\n=========================================\n";
                cout << "            MENU ADMIN GUDANG\n";
                cout << "=========================================\n";

                cout << "1. Input Barang" << endl;
                cout << "2. Tampilkan Barang" << endl;
                cout << "3. Hapus Barang" << endl;
                cout << "4. Update Barang" << endl;
                cout << "5. Cari Barang" << endl;
                cout << "6. Log Aktivitas" << endl;
                cout << "0. Logout" << endl;

                cout << "-----------------------------------------\n";
                cout << "Pilih menu : ";

                cin >> pilihanGudang;
                cin.ignore();

                switch (pilihanGudang) {

                    case 1:
                        tambah_barang();
                        break;

                    case 2:
                        tampilkan_barang();
                        break;

                    case 3:
                        hapus_barang();
                        break;

                    case 4:
                        update_barang();
                        break;

                    case 5:
                        cari_barang();
                        break;

                    case 6:
                        tampilkan_log_barang();
                        break;

                    case 0:
                        break;

                    default:
                        cout << "\nMenu tidak valid!\n";
                }

                if (pilihanGudang == 0) {
                    break;
                }

                cout << "\nTekan ENTER untuk kembali...";
                cin.get();
            }
        }

        // ================= ADMIN KASIR =================
        else if (roleUser == "Kasir") {

            int pilihanKasir;

            while (true) {

                bersihkan_layar();

                cout << "\n=========================================\n";
                cout << "            MENU KASIR\n";
                cout << "=========================================\n";

                cout << "1. Transaksi Kasir" << endl;
                cout << "2. Tampilkan Barang" << endl;
                cout << "3. Riwayat Transaksi" << endl;
                cout << "4. Log Aktivitas" << endl;
                cout << "0. Logout" << endl;

                cout << "-----------------------------------------\n";
                cout << "Pilih menu : ";

                cin >> pilihanKasir;
                cin.ignore();

                switch (pilihanKasir) {

                    case 1:
                        transaksi_kasir();
                        break;

                    case 2:
                        tampilkan_barang();
                        break;

                    case 3:
                        tampilkan_riwayat_transaksi();
                        break;

                    case 4:
                        tampilkan_log_barang();
                        break;

                    case 0:
                        break;

                    default:
                        cout << "\nMenu tidak valid!\n";
                }

                if (pilihanKasir == 0) {
                    break;
                }

                cout << "\nTekan ENTER untuk kembali...";
                cin.get();
            }
        }

        // ================= CUSTOMER =================
        else if (roleUser == "Customer") {

            int pilihanCustomer;

            while (true) {

                bersihkan_layar();

                cout << "\n=========================================\n";
                cout << "              MENU CUSTOMER\n";
                cout << "=========================================\n";

                cout << "1. Lihat Barang" << endl;
                cout << "2. Cari Barang" << endl;
                cout << "3. Tambah Keranjang" << endl;
                cout << "4. Tampilkan Keranjang" << endl;
                cout << "5. Hapus Keranjang" << endl;
                cout << "6. Riwayat Pembelian" << endl;

                cout << "0. Logout" << endl;

                cout << "-----------------------------------------\n";
                cout << "Pilih menu : ";

                cin >> pilihanCustomer;
                cin.ignore();

                switch (pilihanCustomer) {

                    case 1:
                        tampilkan_barang();
                        break;

                    case 2:
                        cari_barang_customer();
                        break;
                    case 3:
                        tambah_ke_keranjang();
                        break;

                    case 4:
                        tampilkan_keranjang();
                        break;

                    case 5:
                        hapus_keranjang();
                        break;

                    case 6:
                        tampilkan_riwayat_customer("customer");
                         break;

                    case 0:
                        break;

                    default:
                        cout << "\nMenu tidak valid!\n";
                }

                if (pilihanCustomer == 0) {
                    break;
                }

                cout << "\nTekan ENTER untuk kembali...";
                cin.get();
            }
        }
    }

    return 0;
}
