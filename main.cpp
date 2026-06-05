// projek: sistem manajemen stok barang minimarket
// materi yang harus ada: variable, tipe data, array, structure, pointer, single linked list, double linked list, 

// ============================================================
// HEADER / LIBRARY INCLUDES
// ============================================================
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

// ===============================
// MENU CUSTOMER
// ===============================

struct QueueCustomer {
    string nama_customer;
    QueueCustomer* next;
};

struct StackTransaksi {
    string receipt;
    StackTransaksi* next;
};

struct User {
    string username;
    string password;
    string role;
    string nama_lengkap;
    string alamat;
    string no_telp;
};

// ============================================================
// DEKLARASI GLOBAL VARIABEL
// ============================================================
struct Barang {
    int id;
    string kode_barang;
    string nama;
    string kategori;
    double harga_beli;
    double harga_jual;
    int stok;
    string satuan;
    string tanggal_kadaluarsa;
    string supplier;
} br;

struct NodeBarang {
    Barang data;
    NodeBarang* next;
};

struct KategoriNode {
    string nama;
    string kode_kat;  // misal: CAT001, CAT002
    int level;             // depth level
    KategoriNode* first_child;  // anak pertama
    KategoriNode* next_sibling; // saudara berikutnya
    KategoriNode* parent;      // orang tua
    
    KategoriNode(string nama, int lvl = 0) {
        this->nama = nama;
        this->level = lvl;
        this->first_child = NULL;
        this->next_sibling = NULL;
        this->parent = NULL;
        
        // generate kode kategori sederhana
        static int counter = 1;
        char buffer[20];
        sprintf(buffer, "KAT%03d", counter++);
        this->kode_kat = string(buffer);
    }
};

struct LogBarang {
    string waktu;
    string aksi;
    string detail;
    LogBarang* next;
    LogBarang* prev;
};

struct Keranjang {
    string nama_barang;
    int jumlah;
    double harga;
    Keranjang* next;
};

struct RiwayatCustomer {
    string nama_customer;
    string barang;
    int jumlah;
    double total;
    string status;
    RiwayatCustomer* next;
};
NodeBarang* head = NULL;
NodeBarang* tail = NULL;
int next_id = 1;

LogBarang* logHead = NULL;
LogBarang* logTail = NULL;

KategoriNode* root_kat = NULL;

Keranjang* headKeranjang = NULL;
Keranjang* tailKeranjang = NULL;

RiwayatCustomer* headRiwayat = NULL;
RiwayatCustomer* tailRiwayat = NULL;

QueueCustomer* frontQueue = NULL;
QueueCustomer* rearQueue = NULL;

StackTransaksi* topTransaksi = NULL;

User users[100];
int jumlahUser = 0;
int user_count = 0; // alias counter pengguna (beberapa bagian kode menggunakan nama ini)
string current_username = "";

// Alias pointer/nama agar kode lama tetap berfungsi
QueueCustomer* &head_queue = frontQueue;
RiwayatCustomer* &head_riwayat = headRiwayat;
Keranjang* &head_keranjang = headKeranjang;
Keranjang* &tail_keranjang = tailKeranjang;
RiwayatCustomer* &tail_riwayat = tailRiwayat;

// Forward declarations for functions defined later in the file
void enqueueCustomer(string nama);
void dequeueCustomer();
void tambah_log(string aksi, string detail);
void _user();
void bersihkan_layar();

// Simple wrappers to adapt naming differences in the codebase
void enqueue(string nama) { enqueueCustomer(nama); }
void dequeue() { dequeueCustomer(); }

// FUNGSI LOGIN SEDERHANA
void register_customer();
string login() {
    while (true) {
        bersihkan_layar();
        cout << "=========================================" << endl;
        cout << "              MENU LOGIN                " << endl;
        cout << "=========================================" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register (Customer)" << endl;
        cout << "0. Keluar" << endl;
        cout << "Pilih: ";

        int opsi;
        cin >> opsi;
        cin.ignore();

        if (opsi == 0) return string("Keluar");
        else if (opsi == 2) { register_customer(); continue; }
        else if (opsi == 1) {
            string uname, pwd;
            cout << "Username: ";
            getline(cin, uname);
            cout << "Password: ";
            getline(cin, pwd);

            for (int i = 0; i < user_count; i++) {
                if (users[i].username == uname && users[i].password == pwd) {
                    current_username = uname;
                    return users[i].role;
                }
            }

            cout << "[!] Login gagal — username/password salah. Tekan ENTER untuk coba lagi...";
            cin.get();
        }
    }
}

// ============================================================
// FUNGSI UTILITY / BANTUAN
// ============================================================
void bersihkan_layar() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

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

// ===============================
// AUTENTIKASI
// ===============================
void init_kat() {
	root_kat = new KategoriNode("SEMUA BARANG", 0);
	
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

    users[0] = {"gudang", "123", "Admin Gudang", "Admin Gudang Utama", "Gudang Pusat", "08123456789"};
    users[1] = {"kasir", "123", "Kasir", "Kasir Utama", "Counter 1", "08987654321"};
    user_count = 2;
}

void tampilkan_kategori_tree(KategoriNode* node, string prefix = "", bool isLast = true) {
    if (node == NULL) return;
    
    cout << prefix;
    cout << (isLast ? "└── " : "├── ");
    cout << node->nama << " (" << node->kode_kat << ")" << endl;
    
    string childPrefix = prefix + (isLast ? "    " : "│   ");
    
    KategoriNode* child = node->first_child;
    while (child != NULL) {
        bool isLastChild = (child->next_sibling == NULL);
        tampilkan_kategori_tree(child, childPrefix, isLastChild);
        child = child->next_sibling;
    }
}

void register_customer() {
    bersihkan_layar();
    cout << "=========================================\n";
    cout << "           REGISTER CUSTOMER BARU        \n";
    cout << "=========================================\n";

    if (user_count >= 100) {
        cout << "[!] Kuota pendaftaran pengguna sudah penuh!\n";
        return;
    }

    string username, password, nama, alamat, no_telp;

    // Membersihkan sisa enter dari menu utama agar getline tidak ter-skip
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Masukkan Nama Lengkap   : ";
    getline(cin, nama);
    cout << "Masukkan Alamat         : ";
    getline(cin, alamat);
    cout << "Masukkan Nomor Telepon  : ";
    getline(cin, no_telp);
    
    // 1. Bagian Cek Username Duplikat
    while (true) {
        cout << "Masukkan Username Baru  : ";
        getline(cin, username);

        bool ada = false;
        for (int i = 0; i < user_count; i++) {
            if (users[i].username == username) { // FIX: Pakai users
                ada = true;
                break;
            }
        }

        if (ada) {
            cout << "[!] Username sudah digunakan. Silakan cari username lain!\n\n";
        } else {
            break;
        }
    }

    cout << "Masukkan Password       : ";
    getline(cin, password);

    // 2. Bagian Penyimpanan Data Baru (FIX: Semua ganti ke users)
    users[user_count].username = username;
    users[user_count].password = password;
    users[user_count].role = "Customer";
    users[user_count].nama_lengkap = nama;
    users[user_count].alamat = alamat;
    users[user_count].no_telp = no_telp;

    user_count++;

    tambah_log("Register", "Customer baru terdaftar: " + username + " (" + nama + ")");

    cout << "-----------------------------------------\n";
    cout << "[✓] Registrasi Berhasil!\n";
    cout << "Silakan gunakan username dan password Anda untuk Login.\n";
}

// ============================================================
// KATEGORI BARANG
// ============================================================


// Mencari kategori berdasarkan kode
KategoriNode* cari_kategori_by_kode(KategoriNode* node, string kode) {
    if (node == NULL) return NULL;
    if (node->kode_kat == kode) return node;
    
    KategoriNode* found = cari_kategori_by_kode(node->first_child, kode);
    if (found != NULL) return found;
    
    return cari_kategori_by_kode(node->next_sibling, kode);
}

// Mendapatkan path lengkap kategori (root > child > subchild)
string get_kategori_path(KategoriNode* node) {
    if (node == NULL) return "";
    if (node->parent == NULL || node->nama == "SEMUA BARANG") 
        return node->nama;
    
    return get_kategori_path(node->parent) + " > " + node->nama;
}

// Menampilkan kategori berdasarkan level
void tampilkan_kategori_by_level(KategoriNode* node, int targetLevel, int currentLevel = 0) {
    if (node == NULL) return;
    
    if (currentLevel == targetLevel && node->nama != "SEMUA BARANG") {
        string indent = "";
        for (int i = 0; i < currentLevel; i++) indent += "  ";
        cout << indent << "• " << node->nama << " (" << node->kode_kat << ")" << endl;
    }
    
    tampilkan_kategori_by_level(node->first_child, targetLevel, currentLevel + 1);
    tampilkan_kategori_by_level(node->next_sibling, targetLevel, currentLevel);
}

void tampilkan_leaf_kategori(KategoriNode* node) {
    if (node == NULL) return;
    
    // Jika tidak punya anak dan bukan root, dia leaf
    if (node->first_child == NULL && node->nama != "SEMUA BARANG") {
        cout << "  • " << get_kategori_path(node) << endl;
    }
    
    tampilkan_leaf_kategori(node->first_child);
    tampilkan_leaf_kategori(node->next_sibling);
}

void pilih_kategori_dari_tree(string &kategori_terpilih) {
    cout << "\n=== DAFTAR KATEGORI (LEAF) ===\n";
    cout << "(Hanya kategori paling bawah yang bisa dipilih)\n\n";
    tampilkan_leaf_kategori(root_kat);
    
    string input_kode;
    bool found = false;
    
    while (!found) {
        cout << "\nMasukkan KODE kategori yang dipilih: ";
        cin >> input_kode;
        
        // Cari kategori berdasarkan kode
        KategoriNode* kat = cari_kategori_by_kode(root_kat, input_kode);
        
        if (kat != NULL && kat->first_child == NULL) {
            kategori_terpilih = kat->nama;
            cout << "\n✓ Kategori terpilih: " << kategori_terpilih << " (" << kat->kode_kat << ")" << endl;
            found = true;
        } else if (kat != NULL && kat->first_child != NULL) {
            cout << "\n✗ '" << kat->nama << "' BUKAN kategori leaf (masih memiliki subkategori)!" << endl;
            cout << "  Silakan pilih kategori paling bawah (tidak memiliki anak).\n";
        } else {
            cout << "\n✗ Kode kategori '" << input_kode << "' tidak ditemukan!" << endl;
            cout << "  Silakan masukkan kode yang benar dari daftar.\n";
        }
    }
}

// ============================================================
// LOG BARANG
// ============================================================
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

// =========================
// ADMIN GUDANG
// ========================
// ============================================================
// CRUD BARANG
// ============================================================
void tambah_barang() {
    bersihkan_layar();
    int jumlah;
    
    cout << "Jumlah barang yang ingin di-input: ";
    cin >> jumlah;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "\n=== TAMBAH BARANG ===\n";
    
    for (int i = 0; i < jumlah; i++) {
        Barang br;
        cout << "\nMasukkan data ke-" << (i+1) << ": \n";
        
        br.id = next_id++;
        br.kode_barang = generate_kode_barang();
        
        cout << "Kode barang (otomatis): " << br.kode_barang << endl;
        
        cout << "> Nama barang: "; 
        getline(cin, br.nama);
        while(br.nama.empty()) {
            cout << "Nama tidak boleh kosong! Masukkan lagi: ";
            getline(cin, br.nama);
        }
        
        // Pilih kategori dari tree
//        pilih_kategori_dari_tree(br.kategori);
        
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

        tambah_log("Tambah Barang", "Kode: " + new_node->data.kode_barang + " | Nama: " + new_node->data.nama + " | Kategori: " + new_node->data.kategori);
        
        cout << "\n[Barang ke-" << (i+1) << " berhasil ditambahkan!]" << endl;
        cout << "\n" << string(100, '-') << endl;
    }
}

void tampilkan_barang_dengan_filter() {
    if (is_kosong()) {
        cout << "List Barang kosong." << endl;
        return;
    }
    
    int filter_by;
    string filter_value;
    NodeBarang* current = head;
    int no = 1;
    bool found = false;
    
    cout << "\n=== FILTER BARANG ===\n";
    cout << "1. Tampilkan semua barang\n";
    cout << "2. Filter berdasarkan Kategori\n";
    cout << "3. Filter berdasarkan Nama Barang\n";
    cout << "4. Filter berdasarkan Stok (<= nilai tertentu)\n";
    cout << "5. Filter berdasarkan Stok (>= nilai tertentu)\n";
    cout << "6. Filter berdasarkan Harga Jual (<= nilai tertentu)\n";
    cout << "7. Filter berdasarkan Harga Jual (>= nilai tertentu)\n";
    cout << "8. Filter berdasarkan Supplier\n";
    cout << "9. Tampilkan barang yang akan kadaluarsa (<= 30 hari)\n";
    cout << "Pilih filter: ";
    cin >> filter_by;
    cin.ignore();
    
    // Handle filter 9 (kadaluarsa) tanpa input nilai tambahan
    if (filter_by == 9) {
        // Tidak perlu input nilai, langsung proses
    }
    // Handle filter lainnya yang perlu input nilai
    else if (filter_by != 1) {
        if (filter_by == 2) cout << "Masukkan nama kategori: ";
        else if (filter_by == 3) cout << "Masukkan nama barang: ";
        else if (filter_by == 4 || filter_by == 5) cout << "Masukkan nilai stok: ";
        else if (filter_by == 6 || filter_by == 7) cout << "Masukkan nilai harga: ";
        else if (filter_by == 8) cout << "Masukkan nama supplier: ";
        
        getline(cin, filter_value);
    }
    
    // Konversi ke lowercase untuk filter
    string filter_lower = toLowerCase(filter_value);
    
    cout << "\n=== HASIL FILTER BARANG ===\n";
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
    
    current = head;
    no = 1;
    
    // Dapatkan tanggal sekarang untuk filter kadaluarsa
    time_t t = time(0);
    tm* now = localtime(&t);
    int curr_d = now->tm_mday;
    int curr_m = now->tm_mon + 1;
    int curr_y = now->tm_year + 1900;
    
    while (current != NULL) {
        bool tampil = false;
        
        switch(filter_by) {
            case 1: // Tampilkan semua
                tampil = true;
                break;
                
            case 2: // Filter kategori
                if (toLowerCase(current->data.kategori) == filter_lower) {
                    tampil = true;
                }
                break;
                
            case 3: // Filter nama (contains)
                if (toLowerCase(current->data.nama).find(filter_lower) != string::npos) {
                    tampil = true;
                }
                break;
                
            case 4: // Filter stok <=
                if (current->data.stok <= atoi(filter_value.c_str())) {
                    tampil = true;
                }
                break;
                
            case 5: // Filter stok >=
                if (current->data.stok >= atoi(filter_value.c_str())) {
                    tampil = true;
                }
                break;
                
            case 6: // Filter harga jual <=
                if (current->data.harga_jual <= atof(filter_value.c_str())) {
                    tampil = true;
                }
                break;
                
            case 7: // Filter harga jual >=
                if (current->data.harga_jual >= atof(filter_value.c_str())) {
                    tampil = true;
                }
                break;
                
            case 8: // Filter supplier
                if (toLowerCase(current->data.supplier) == filter_lower) {
                    tampil = true;
                }
                break;
                
            case 9: { // Filter kadaluarsa <= 30 hari
                if (current->data.tanggal_kadaluarsa != "-") {
                    int d = atoi(current->data.tanggal_kadaluarsa.substr(0,2).c_str());
                    int m = atoi(current->data.tanggal_kadaluarsa.substr(3,2).c_str());
                    int y = atoi(current->data.tanggal_kadaluarsa.substr(6,4).c_str());
                    
                    // Hitung selisih hari
                    tm expire_tm = {0};
                    expire_tm.tm_mday = d;
                    expire_tm.tm_mon = m - 1;
                    expire_tm.tm_year = y - 1900;
                    
                    time_t expire_time = mktime(&expire_tm);
                    double diff_seconds = difftime(expire_time, t);
                    int diff_days = diff_seconds / (60 * 60 * 24);
                    
                    if (diff_days >= 0 && diff_days <= 30) {
                        tampil = true;
                    }
                }
                break;
            }
            
            default:
                tampil = true;
                break;
        }
        
        if (tampil) {
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
            found = true;
        }
        
        current = current->next;
    }
    
    if (!found) {
        cout << left << setw(148) << "Tidak ada barang yang sesuai dengan filter!" << endl;
    }
    
    cout << string(148, '=') << endl;
}

void tampilkan_barang() {
    while (true) {
        bersihkan_layar();

        if (is_kosong()) {
            cout << "List Barang kosong." << endl;
            cout << "\nTekan ENTER untuk kembali...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            return;
        }

        int pilih;

        cout << "\n=== MENU TAMPILAN BARANG ===\n";
        cout << "1. Tampilkan semua barang\n";
        cout << "2. Tampilkan dengan filter\n";
        cout << "0. Kembali\n";
        cout << "Pilih: ";
        cin >> pilih;
        cin.ignore();

        if (pilih == 0) {
            return;
        }

        else if (pilih == 1) {
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
                     << setw(12) << current->data.harga_beli
                     << setw(12) << current->data.harga_jual
                     << setw(8)  << current->data.stok
                     << setw(10) << current->data.satuan
                     << setw(15) << current->data.tanggal_kadaluarsa
                     << current->data.supplier << endl;

                current = current->next;
            }

            cout << string(148, '=') << endl;

            cout << "\nTekan ENTER untuk kembali...";
            cin.get();
        }

        else if (pilih == 2) {
            tampilkan_barang_dengan_filter();

            cout << "\nTekan ENTER untuk kembali...";
            cin.get();
        }

        else {
            cout << "\nPilihan tidak valid!";
            cout << "\nTekan ENTER untuk coba lagi...";
            cin.get();
        }
    }
}

void hapus_barang() {
    //bersihkan_layar();
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


// ==============================
// ini adalah fungsi admin kasir
// ==============================
//  Kasir men input data untuk costumer yang melakukan transaksi secara OFFLINE
// queue dan STackk

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

void update_status_pesanan() {

    if(headRiwayat == NULL){
        cout << "Belum ada pesanan.\n";
        return;
    }

    RiwayatCustomer* current = headRiwayat;
    int no = 1;

    cout << "\n=== DAFTAR PESANAN ===\n";

    while(current != NULL){
        cout << no << ". "
             << current->barang
             << " | Customer: " << current->nama_customer
             << " | Status: " << current->status
             << endl;

        current = current->next;
        no++;
    }

    int pilih;
    cout << "\nPilih nomor pesanan: ";
    cin >> pilih;

    current = headRiwayat;

    for(int i=1; i<pilih && current!=NULL; i++){
        current = current->next;
    }

    if(current != NULL){
        current->status = "Selesai";
        cout << "\nStatus pesanan berhasil diubah menjadi Selesai!\n";
    }
}

void transaksi_kasir() {
    bersihkan_layar();

    // Validasi antrean menggunakan 'frontQueue' bawaan kelompokmu
    if (frontQueue == NULL) {
        cout << "=========================================\n";
        cout << "         SISTEM PEMBAYARAN KASIR        \n";
        cout << "=========================================\n";
        cout << "\n[!] Tidak ada antrean customer saat ini.\n";
        return;
    }

    // Mengambil data customer terdepan dari antrean
    string namaCustomer = frontQueue->nama_customer;

    cout << "=========================================\n";
    cout << "         SISTEM PEMBAYARAN KASIR        \n";
    cout << "=========================================\n";
    cout << "Melayani Customer: " << namaCustomer << endl;
    cout << "-----------------------------------------\n";

    // Melacak daftar pesanan menggunakan 'headRiwayat' bawaan kelompokmu
    RiwayatCustomer* currentPesanan = headRiwayat;
    double grandTotal = 0;
    int no = 1;

    cout << "Daftar Belanjaan:\n";
    while (currentPesanan != NULL) {
        if (toLowerCase(currentPesanan->nama_customer) == toLowerCase(namaCustomer) && currentPesanan->status == "Diproses") {
            cout << no++ << ". " << currentPesanan->barang 
                 << " (x" << currentPesanan->jumlah << ")"
                 << " | Total: Rp" << currentPesanan->total << endl;
            grandTotal += currentPesanan->total;
        }
        currentPesanan = currentPesanan->next;
    }

    if (grandTotal == 0) {
        cout << "[!] Data belanjaan customer tidak ditemukan atau sudah dibayar.\n";
        dequeueCustomer(); 
        return;
    }

    cout << "-----------------------------------------\n";
    cout << "TOTAL YANG HARUS DIBAYAR: Rp" << grandTotal << endl;
    cout << "-----------------------------------------\n";

    double uangDibayar = 0;
    while (true) {
        cout << "Masukkan Uang Pembayaran: Rp";
        cin >> uangDibayar;
        cin.ignore();

        if (uangDibayar >= grandTotal) {
            break;
        } else {
            cout << "[!] Uang tidak cukup! Kurang Rp" << (grandTotal - uangDibayar) << ". Silakan input kembali.\n\n";
        }
    }

    double kembalian = uangDibayar - grandTotal;
    cout << "Kembalian               : Rp" << kembalian << endl;
    cout << "-----------------------------------------\n";

    string receipt =
        "\n=========== MINIMARKET RECEIPT ===========\n"
        "Nama Customer : " + namaCustomer + "\n"
        "Total Belanja : Rp" + to_string((int)grandTotal) + "\n"
        "Uang Tunai    : Rp" + to_string((int)uangDibayar) + "\n"
        "Kembalian     : Rp" + to_string((int)kembalian) + "\n"
        "Status        : LUNAS\n"
        "==========================================";

    cout << receipt << endl;

    // Memasukkan nota belanja ke Stack transaksi kasir bawaan kelompokmu
    pushTransaksi(receipt);

    // Mengubah status riwayat customer di linked list menjadi Selesai
    currentPesanan = headRiwayat;
    while (currentPesanan != NULL) {
        if (toLowerCase(currentPesanan->nama_customer) == toLowerCase(namaCustomer) && currentPesanan->status == "Diproses") {
            currentPesanan->status = "Selesai";
        }
        currentPesanan = currentPesanan->next;
    }

    tambah_log("Pembayaran Kasir", "Pembayaran atas nama " + namaCustomer + " LUNAS sebesar Rp" + to_string((int)grandTotal));

    // Keluarkan customer dari antrean queue setelah pembayaran sukses
    dequeueCustomer();

    cout << "\n[✓] Transaksi berhasil diselesaikan!\n";
}

// =========================================
// ini adalah fungsi customer (online)
// =========================================

// 1. tambahin barang ke keranjang (customer online)
// Pake single linked list

void tambah_riwayat_customer(string nama, string barang, int jumlah, double total, string status) {

    RiwayatCustomer* baru = new RiwayatCustomer();

    baru->nama_customer = nama;
    baru->barang = barang;
    baru->jumlah = jumlah;
    baru->total = total;
    baru->status = "Diproses";
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
    cout << "=========================================\n";
    cout << "       RIWAYAT PEMBELIAN & STATUS        \n";
    cout << "=========================================\n";

    // Validasi apakah stack transaksi kosong atau tidak
    // Sesuaikan 'topStack' jika di kodemu namanya 'top' atau sejenisnya
    if (topTransaksi == NULL) {
        cout << "[!] Belum ada riwayat transaksi di toko ini.\n";
        return;
    }

    // Menggunakan temporary pointer untuk menelusuri Stack tanpa merusak data asli
    // Sesuaikan 'NodeTransaksi' atau 'Stack' dengan nama struct stack kelompokmu
    StackTransaksi* curr = topTransaksi;
    bool ditemukan = false;
    int no = 1;

    while (curr != NULL) {
        // DEBUG FIX: Mencari apakah nama customer ada di dalam teks 'receipt' utuh
        // Fungsi .find() akan mengembalikan nilai npos jika teks tidak ditemukan
        if (curr->receipt.find(namaCustomer) != string::npos) {
            ditemukan = true;
            cout << "Nota Ke-" << no++ << endl;
            cout << "-----------------------------------------\n";
            cout << curr->receipt << endl; // Langsung cetak isi nota utuhnya di sini
            cout << "Status        : LUNAS (Selesai di Kasir)\n";
            cout << "=========================================\n\n";
        }
        curr = curr->next; // Pindah ke tumpukan nota di bawahnya
    }

    if (!ditemukan) {
        cout << "[!] Anda belum memiliki riwayat pembayaran.\n";
        cout << "Silakan lakukan checkout dan selesaikan pembayaran di Kasir.\n";
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

void hapus_riwayat_customer(string namaBarang) {
    RiwayatCustomer* current = headRiwayat;
    RiwayatCustomer* prev = NULL;

    while (current != NULL) {

        if (toLowerCase(current->barang) == toLowerCase(namaBarang)) {

            if (current == headRiwayat) {
                headRiwayat = current->next;
            } else {
                prev->next = current->next;
            }

            if (current == tailRiwayat) {
                tailRiwayat = prev;
            }

            delete current;
            return;
        }

        prev = current;
        current = current->next;
    }
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
    hapus_riwayat_customer(nama);

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

// Menampilkan kategori (preorder traversal dengan indikator garis)
void tampilkan_kategori(KategoriNode* node, string prefix = "", bool isLast = true) {
    if (node == NULL) return;
    
    // Cetak node saat ini
    cout << prefix;
    cout << (isLast ? "+-- " : "+-- ");
    cout << node->nama << " (" << node->kode_kat << ")" << endl;
    
    // Siapkan prefix untuk anak-anak
    string childPrefix = prefix + (isLast ? "    " : "¦   ");
    
    // Rekursif ke anak-anak (first_child dan next_sibling-nya)
    KategoriNode* child = node->first_child;
    while (child != NULL) {
        bool isLastChild = (child->next_sibling == NULL);
        tampilkan_kategori(child, childPrefix, isLastChild);
        child = child->next_sibling;
    }
}

// Mencari kategori berdasarkan nama (dengan path lengkap)
KategoriNode* cari_kategori(KategoriNode* node, string nama) {
    if (node == NULL) return NULL;
    if (node->nama == nama) return node;
    
    // Cari di anak
    KategoriNode* found = cari_kategori(node->first_child, nama);
    if (found != NULL) return found;
    
    // Cari di saudara
    return cari_kategori(node->next_sibling, nama);
}

// Menambah kategori baru di bawah kategori induk
bool tambah_kategori(string nama_induk, string nama_baru) {
    if (root_kat == NULL) {
        cout << "Tree kategori belum diinisialisasi!\n";
        return false;
    }
    
    KategoriNode* parent = cari_kategori(root_kat, nama_induk);
    if (parent == NULL) {
        cout << "Kategori induk '" << nama_induk << "' tidak ditemukan!\n";
        return false;
    }
    
    // Cek apakah nama sudah ada di level yang sama
    KategoriNode* existing = parent->first_child;
    while (existing != NULL) {
        if (existing->nama == nama_baru) {
            cout << "Kategori '" << nama_baru << "' sudah ada di bawah '" << nama_induk << "'!\n";
            return false;
        }
        existing = existing->next_sibling;
    }
    
    KategoriNode* baru = new KategoriNode(nama_baru, parent->level + 1);
    baru->parent = parent;
    
    // Tambahkan sebagai anak terakhir
    if (parent->first_child == NULL) {
        parent->first_child = baru;
    } else {
        KategoriNode* sibling = parent->first_child;
        while (sibling->next_sibling != NULL) {
            sibling = sibling->next_sibling;
        }
        sibling->next_sibling = baru;
    }
    
    cout << "Kategori '" << nama_baru << "' berhasil ditambahkan di bawah '" << nama_induk << "'\n";
    return true;
}

// ===============================
// MENU ADMIN GUDANG
// ===============================
void menu_admin_gudang() {
    int pilihan;
    
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
        cout << "7. Manajemen Kategori Barang" << endl;
        cout << "0. Logout" << endl;
        cout << "-----------------------------------------\n";
        cout << "Pilih menu : ";
        
        cin >> pilihan;
        cin.ignore();
        
        switch (pilihan) {
            case 1: tambah_barang(); break;
            case 2: tampilkan_barang(); break;
            case 3: hapus_barang(); break;
            case 4: update_barang(); break;
            case 5: cari_barang(); break;
            case 6: tampilkan_log_barang(); break;
//            case 7: menu_kategori(); break;
            case 0: 
                cout << "Logout berhasil!\n";
                return;
            default:
                cout << "\nMenu tidak valid!\n";
        }
        
        if (pilihan != 0) {
            cout << "\nTekan ENTER untuk kembali...";
            cin.get();
        }
    }
}

// ===============================
// MENU KASIR
// ===============================
void menu_kasir() {
    int pilihan;
    
    while (true) {
        bersihkan_layar();
        
        cout << "\n=========================================\n";
        cout << "              MENU KASIR\n";
        cout << "=========================================\n";
        cout << "1. Transaksi Kasir" << endl;
        cout << "2. Tampilkan Barang" << endl;
        cout << "3. Riwayat Transaksi" << endl;
        cout << "4. Log Aktivitas" << endl;
        cout << "5. Update Status" << endl;
        cout << "0. Logout" << endl;
        cout << "-----------------------------------------\n";
        cout << "Pilih menu : ";
        
        cin >> pilihan;
        cin.ignore();
        
        switch (pilihan) {
            case 1: transaksi_kasir(); break; // Aman, langsung memanggil fungsi otomatis Rafa yang sudah di-debug tadi
            case 2: tampilkan_barang(); break;
            case 3: tampilkan_riwayat_transaksi(); break;
            case 4: tampilkan_log_barang(); break;
            case 5: update_status_pesanan(); break;
            case 0:
                cout << "Logout berhasil!\n";
                return;
            default:
                cout << "\nMenu tidak valid!\n";
        }
        
        if (pilihan != 0) {
            cout << "\nTekan ENTER untuk kembali...";
            cin.get();
        }
    }
}

void checkout_keranjang() {
    bersihkan_layar();

    if (head_keranjang == NULL) {
        cout << "=========================================\n";
        cout << "            CHECKOUT PESANAN             \n";
        cout << "=========================================\n";
        cout << "\n[!] Keranjang Anda masih kosong. Silakan belanja terlebih dahulu.\n";
        return;
    }

    cout << "=========================================\n";
    cout << "            CHECKOUT PESANAN             \n";
    cout << "=========================================\n";
    cout << "Nama Customer : " << current_username << endl;
    cout << "-----------------------------------------\n";

    Keranjang* current = head_keranjang;
    int no = 1;
    double grandTotal = 0;

    while (current != NULL) {
        double subtotal = current->jumlah * current->harga;
        cout << no++ << ". " << current->nama_barang 
             << " (x" << current->jumlah << ")"
             << " | Harga: Rp" << current->harga 
             << " | Subtotal: Rp" << subtotal << endl;
        
        grandTotal += subtotal;
        current = current->next;
    }

    cout << "-----------------------------------------\n";
    cout << "Total yang harus dibayar: Rp" << grandTotal << endl;
    cout << "-----------------------------------------\n";

    char konfirmasi;
    cout << "Apakah Anda yakin ingin melakukan Checkout? (y/n): ";
    cin >> konfirmasi;
    cin.ignore();

    if (konfirmasi == 'y' || konfirmasi == 'Y') {
        // 1. Kurangi stok barang utama di Gudang/Toko
        Keranjang* currKeranjang = headKeranjang;
        while (currKeranjang != NULL) {
            NodeBarang* currBarang = head;
            while (currBarang != NULL) {
                if (toLowerCase(currBarang->data.nama) == toLowerCase(currKeranjang->nama_barang)) {
                    currBarang->data.stok -= currKeranjang->jumlah;
                    break;
                }
                currBarang = currBarang->next;
            }
            currKeranjang = currKeranjang->next;
        }

        // =========================================================================
        // JEMBATAN EMAS: Salin data dari Keranjang ke headRiwayat (Biar Kasir bisa baca)
        // =========================================================================
        currKeranjang = headKeranjang;
        while (currKeranjang != NULL) {
            RiwayatCustomer* baru = new RiwayatCustomer();
            baru->nama_customer = current_username;
            baru->barang = currKeranjang->nama_barang;
            baru->jumlah = currKeranjang->jumlah;
            baru->total = currKeranjang->jumlah * currKeranjang->harga;
            baru->status = "Diproses"; // Kunci agar dibaca oleh kasir
            baru->next = NULL;

            // Masukkan ke Linked List headRiwayat kelompokmu
            if (headRiwayat == NULL) {
                headRiwayat = baru;
            } else {
                RiwayatCustomer* temp = headRiwayat;
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = baru;
            }
            currKeranjang = currKeranjang->next;
        }

        // 2. Memasukkan customer ke Queue Antrean Kasir
        enqueueCustomer(current_username);

        // 3. Mengosongkan isi keranjang belanja setelah disalin
        currKeranjang = headKeranjang;
        while (currKeranjang != NULL) {
            Keranjang* hapus = currKeranjang;
            currKeranjang = currKeranjang->next;
            delete hapus;
        }
        headKeranjang = NULL;
        tailKeranjang = NULL;

        tambah_log("Checkout Customer", "Customer " + current_username + " berhasil checkout sebesar Rp" + to_string((int)grandTotal));

        cout << "\n[✓] Checkout Berhasil!\n";
        cout << "Pesanan Anda telah diteruskan ke Kasir.\n";
        cout << "Silakan menuju ke Kasir untuk mengantre dan melakukan pembayaran.\n";
    } else {
        cout << "\n[!] Checkout dibatalkan. Barang tetap berada di keranjang Anda.\n";
    }
}

// ===============================
// MENU CUSTOMER
// ===============================
// ===============================
void menu_customer() {
    int pilihan;
    
    // Menarik biodata pelengkap dari array 'user' berdasarkan siapa yang sedang login
    User userSekarang;
    for (int i = 0; i < user_count; i++) {
        if (users[i].username == current_username) {
            userSekarang = users[i];
            break;
        }
    }
    
    while (true) {
        bersihkan_layar();
        
        cout << "\n=========================================\n";
        cout << "            MENU CUSTOMER\n";
        cout << "=========================================\n";
        cout << "Selamat Datang, " << userSekarang.nama_lengkap << "!\n";
        cout << "Alamat Kirim  : " << userSekarang.alamat << "\n";
        cout << "No. Telepon   : " << userSekarang.no_telp << "\n";
        cout << "-----------------------------------------\n";
        cout << "1. Lihat Barang" << endl;
        cout << "2. Cari Barang" << endl;
        cout << "3. Tambah Keranjang" << endl;
        cout << "4. Tampilkan Keranjang" << endl;
        cout << "5. Hapus Keranjang" << endl;
        cout << "6. CHECKOUT PESANAN (Rafa)" << endl; // Mengaktifkan menu checkout Rafa
        cout << "7. Riwayat Pembelian & Status" << endl;
        cout << "0. Logout" << endl;
        cout << "-----------------------------------------\n";
        cout << "Pilih menu : ";
        
        cin >> pilihan;
        cin.ignore();
        
        switch (pilihan) {
            case 1: tampilkan_barang(); break;
            case 2: cari_barang_customer(); break;
            case 3: tambah_ke_keranjang(); break;
            case 4: tampilkan_keranjang(); break;
            case 5: hapus_keranjang(); break;
            case 6: checkout_keranjang(); break; // Memanggil fitur checkout Rafa
            case 7: tampilkan_riwayat_customer(current_username); break; 
            case 0:
                cout << "Logout berhasil!\n";
                current_username = ""; 
                return;
            default:
                cout << "\nMenu tidak valid!\n";
        }
        
        if (pilihan != 0) {
            cout << "\nTekan ENTER untuk kembali...";
            cin.get();
        }
    }
}
// ===============================
// MAIN PROGRAM
// ===============================
int main() {
    // Inisialisasi data awal
    init_kat();
    
    while (true) {
        string roleUser = login();
        
        // Cek apakah user memilih keluar
        if (roleUser == "Keluar") {
            cout << "\nTerima kasih telah menggunakan program!\n";
            break;
        }
        
        // Redirect ke menu sesuai role
        if (roleUser == "Admin Gudang") {
            menu_admin_gudang();
        }
        else if (roleUser == "Kasir") {
            menu_kasir();
        }
        else if (roleUser == "Customer") {
            menu_customer(); 
        }
    }
    return 0;
}
 
