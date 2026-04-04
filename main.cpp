#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <limits> 
#include <ctime>
using namespace std;

// projek uts: sistem manajemen stok barang minimarket
// materi yang harus ada: variable, tipe data, array, structure, pointer, single linked list, double linked list, 

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
};

struct NodeBarang {
	Barang data;
	NodeBarang* next;
};

NodeBarang* head = NULL;
NodeBarang* tail = NULL;
int next_id = 1; // auto increment id

void tambah_barang();
void tampilkan_barang();
bool is_kosong();
string generate_kode_barang(int id);
//void edit_barang();

Barang br;

int main() {
	tambah_barang();
	tampilkan_barang();
}

bool is_kosong() {
    return head == NULL;
}

string generate_kode_barang(int id) {
	char buffer[20];
    sprintf(buffer, "BRG%04d", id); 
    return string(buffer);
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
    int jumlah;
    
    cout << "Jumlah barang yang ingin di-input: ";
    cin >> jumlah;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "\n=== TAMBAH BARANG ===\n";
    
    for (int i = 0; i < jumlah; i++) {
        cout << "\nMasukkan data ke-" << (i+1) << ": \n";
        
        br.id = next_id++;
        br.kode_barang = generate_kode_barang(br.id);
        
        cout << "Kode barang (otomatis): " << br.kode_barang << endl;
        
        cout << "> Nama barang: "; 
		getline(cin, br.nama);
		while(br.nama.empty()) {
    		cout << "Nama tidak boleh kosong! Masukkan lagi: ";
    		getline(cin, br.nama);
		}
        
        cout << "> Kategori: "; 
        getline(cin, br.kategori);
        if(br.kategori.empty()) br.kategori = "Lainnya";
        
        cout << "> Harga beli: ";
		while(!(cin >> br.harga_beli)) {
    		cout << "Input harus angka! Masukkan lagi: ";
    		cin.clear();
    		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
        
        cout << "> Harga jual: ";
		while(!(cin >> br.harga_jual)) {
    		cout << "Input harus angka! Masukkan lagi: ";
    		cin.clear();
    		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
        
        cout << "> Stok awal: ";
		while(!(cin >> br.stok)) {
    		cout << "Input harus angka! Masukkan lagi: ";
    		cin.clear();
    		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
        
        cout << "> Satuan (pcs/botol/pack): "; 
        cin >> br.satuan;
        if(br.satuan.empty()) br.satuan = "pcs";
        
        cout << "> Tanggal kadaluarsa (DD-MM-YYYY) [Enter jika tidak ada]: ";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
        
        cout << "> Supplier: "; 
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
        
        cout << "\n[Barang ke-" << (i+1) << " berhasil ditambahkan!]" << endl;
        cout << "\n" << string(100, '-') << endl;
    }
}

void tampilkan_barang() {
    if (is_kosong()) {
        cout << "List Barang kosong." << endl;
        return;
    }

    cout << "\n=== DAFTAR BARANG ===" << endl;
    cout << string(150, '=') << endl;

    cout << left << setw(5)  << "No"
         << setw(8)  << "ID"
         << setw(12) << "Kode"
         << setw(20) << "Nama"
         << setw(15) << "Kategori"
         << setw(12) << "Hrg Beli"
         << setw(12) << "Hrg Jual"
         << setw(8)  << "Stok"
         << setw(10) << "Satuan"
         << setw(15) << "Expired"
         << "Supplier" << endl;

    cout << string(150, '-') << endl;

    NodeBarang* current = head;
    int no = 1;

    while (current != NULL) {
        cout << left << setw(5)  << no++
             << setw(8)  << current->data.id
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

    cout << string(150, '=') << endl;
}
