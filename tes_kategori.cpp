#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
using namespace std;

// Struktur Node untuk Kategori Tree
struct KategoriNode {
    string nama;
    string kode_kategori;  // misal: CAT001, CAT002
    int level;             // depth level
    KategoriNode* firstChild;  // anak pertama
    KategoriNode* nextSibling; // saudara berikutnya
    KategoriNode* parent;      // orang tua
    
    KategoriNode(string nama, int lvl = 0) {
        this->nama = nama;
        this->level = lvl;
        this->firstChild = NULL;
        this->nextSibling = NULL;
        this->parent = NULL;
        
        // generate kode kategori sederhana
        static int counter = 1;
        char buffer[20];
        sprintf(buffer, "CAT%03d", counter++);
        this->kode_kategori = string(buffer);
    }
};

KategoriNode* rootKategori = NULL;

// Inisialisasi kategori default dengan struktur baru
void init_kategori_default() {
    rootKategori = new KategoriNode("SEMUA BARANG", 0);
    
    // Level 1 - Kategori Utama
    KategoriNode* makananMinuman = new KategoriNode("Makanan & Minuman", 1);
    KategoriNode* rumahTangga = new KategoriNode("Kebutuhan Rumah Tangga", 1);
    KategoriNode* perawatanKesehatan = new KategoriNode("Perawatan Diri & Kesehatan", 1);
    KategoriNode* alatTulis = new KategoriNode("Alat Tulis & Kantor", 1);
    KategoriNode* lainnya = new KategoriNode("Lainnya", 1);
    
    // Hubungkan ke root
    rootKategori->firstChild = makananMinuman;
    makananMinuman->nextSibling = rumahTangga;
    rumahTangga->nextSibling = perawatanKesehatan;
    perawatanKesehatan->nextSibling = alatTulis;
    alatTulis->nextSibling = lainnya;
    
    makananMinuman->parent = rootKategori;
    rumahTangga->parent = rootKategori;
    perawatanKesehatan->parent = rootKategori;
    alatTulis->parent = rootKategori;
    lainnya->parent = rootKategori;
    
    // ===== LEVEL 2 - Subkategori Makanan & Minuman =====
    KategoriNode* makananRingan = new KategoriNode("Makanan Ringan", 2);
    KategoriNode* makananInstan = new KategoriNode("Makanan Instan", 2);
    KategoriNode* minuman = new KategoriNode("Minuman", 2);
    
    makananMinuman->firstChild = makananRingan;
    makananRingan->nextSibling = makananInstan;
    makananInstan->nextSibling = minuman;
    
    makananRingan->parent = makananMinuman;
    makananInstan->parent = makananMinuman;
    minuman->parent = makananMinuman;
    
    // ===== LEVEL 2 - Subkategori Kebutuhan Rumah Tangga =====
    KategoriNode* peralatanMandi = new KategoriNode("Peralatan Mandi", 2);
    KategoriNode* alatKebersihan = new KategoriNode("Alat Kebersihan", 2);
    KategoriNode* peralatanDapur = new KategoriNode("Peralatan Dapur", 2);
    
    rumahTangga->firstChild = peralatanMandi;
    peralatanMandi->nextSibling = alatKebersihan;
    alatKebersihan->nextSibling = peralatanDapur;
    
    peralatanMandi->parent = rumahTangga;
    alatKebersihan->parent = rumahTangga;
    peralatanDapur->parent = rumahTangga;
    
    // ===== LEVEL 2 - Subkategori Perawatan Diri & Kesehatan =====
    KategoriNode* perawatanTubuh = new KategoriNode("Perawatan Tubuh", 2);
    KategoriNode* kosmetik = new KategoriNode("Kosmetik", 2);
    KategoriNode* obat = new KategoriNode("Obat-obatan", 2);
    
    perawatanKesehatan->firstChild = perawatanTubuh;
    perawatanTubuh->nextSibling = kosmetik;
    kosmetik->nextSibling = obat;
    
    perawatanTubuh->parent = perawatanKesehatan;
    kosmetik->parent = perawatanKesehatan;
    obat->parent = perawatanKesehatan;
    
    // ===== LEVEL 3 - Sub-subkategori (opsional, bisa ditambahkan) =====
    // Contoh: Menambahkan subkategori di bawah Makanan Ringan
    KategoriNode* keripik = new KategoriNode("Keripik", 3);
    KategoriNode* biskuit = new KategoriNode("Biskuit", 3);
    KategoriNode* coklat = new KategoriNode("Coklat & Permen", 3);
    
    makananRingan->firstChild = keripik;
    keripik->nextSibling = biskuit;
    biskuit->nextSibling = coklat;
    
    keripik->parent = makananRingan;
    biskuit->parent = makananRingan;
    coklat->parent = makananRingan;
    
    // Contoh: Subkategori Minuman
    KategoriNode* airMineral = new KategoriNode("Air Mineral", 3);
    KategoriNode* minumanRingan = new KategoriNode("Minuman Ringan", 3);
    KategoriNode* minumanIsotonic = new KategoriNode("Minuman Isotonik", 3);
    
    minuman->firstChild = airMineral;
    airMineral->nextSibling = minumanRingan;
    minumanRingan->nextSibling = minumanIsotonic;
    
    airMineral->parent = minuman;
    minumanRingan->parent = minuman;
    minumanIsotonic->parent = minuman;
    
    // Contoh: Subkategori Alat Kebersihan
    KategoriNode* deterjen = new KategoriNode("Deterjen & Pewangi", 3);
    KategoriNode* pembersihLantai = new KategoriNode("Pembersih Lantai", 3);
    
    alatKebersihan->firstChild = deterjen;
    deterjen->nextSibling = pembersihLantai;
    
    deterjen->parent = alatKebersihan;
    pembersihLantai->parent = alatKebersihan;
}

// Menampilkan kategori (preorder traversal dengan indikator garis)
void tampilkan_kategori(KategoriNode* node, string prefix = "", bool isLast = true) {
    if (node == NULL) return;
    
    // Cetak node saat ini
    cout << prefix;
    cout << (isLast ? "+-- " : "+-- ");
    cout << node->nama << " (" << node->kode_kategori << ")" << endl;
    
    // Siapkan prefix untuk anak-anak
    string childPrefix = prefix + (isLast ? "    " : "¦   ");
    
    // Rekursif ke anak-anak (firstChild dan nextSibling-nya)
    KategoriNode* child = node->firstChild;
    while (child != NULL) {
        bool isLastChild = (child->nextSibling == NULL);
        tampilkan_kategori(child, childPrefix, isLastChild);
        child = child->nextSibling;
    }
}

// Mencari kategori berdasarkan nama (dengan path lengkap)
KategoriNode* cari_kategori(KategoriNode* node, string nama) {
    if (node == NULL) return NULL;
    if (node->nama == nama) return node;
    
    // Cari di anak
    KategoriNode* found = cari_kategori(node->firstChild, nama);
    if (found != NULL) return found;
    
    // Cari di saudara
    return cari_kategori(node->nextSibling, nama);
}

// Mencari kategori berdasarkan kode
KategoriNode* cari_kategori_by_kode(KategoriNode* node, string kode) {
    if (node == NULL) return NULL;
    if (node->kode_kategori == kode) return node;
    
    KategoriNode* found = cari_kategori_by_kode(node->firstChild, kode);
    if (found != NULL) return found;
    
    return cari_kategori_by_kode(node->nextSibling, kode);
}

// Menambah kategori baru di bawah kategori induk
bool tambah_kategori(string nama_induk, string nama_baru) {
    if (rootKategori == NULL) {
        cout << "Tree kategori belum diinisialisasi!\n";
        return false;
    }
    
    KategoriNode* parent = cari_kategori(rootKategori, nama_induk);
    if (parent == NULL) {
        cout << "Kategori induk '" << nama_induk << "' tidak ditemukan!\n";
        return false;
    }
    
    // Cek apakah nama sudah ada di level yang sama
    KategoriNode* existing = parent->firstChild;
    while (existing != NULL) {
        if (existing->nama == nama_baru) {
            cout << "Kategori '" << nama_baru << "' sudah ada di bawah '" << nama_induk << "'!\n";
            return false;
        }
        existing = existing->nextSibling;
    }
    
    KategoriNode* baru = new KategoriNode(nama_baru, parent->level + 1);
    baru->parent = parent;
    
    // Tambahkan sebagai anak terakhir
    if (parent->firstChild == NULL) {
        parent->firstChild = baru;
    } else {
        KategoriNode* sibling = parent->firstChild;
        while (sibling->nextSibling != NULL) {
            sibling = sibling->nextSibling;
        }
        sibling->nextSibling = baru;
    }
    
    cout << "Kategori '" << nama_baru << "' berhasil ditambahkan di bawah '" << nama_induk << "'\n";
    return true;
}

// Mendapatkan path lengkap kategori (root > child > subchild)
string get_kategori_path(KategoriNode* node) {
    if (node == NULL) return "";
    if (node->parent == NULL || node->nama == "SEMUA BARANG") 
        return node->nama;
    
    return get_kategori_path(node->parent) + " > " + node->nama;
}

// Menampilkan semua leaf (kategori paling bawah) untuk pemilihan barang
void tampilkan_leaf_kategori(KategoriNode* node) {
    if (node == NULL) return;
    
    // Jika tidak punya anak dan bukan root, dia leaf
    if (node->firstChild == NULL && node->nama != "SEMUA BARANG") {
        cout << "  • " << get_kategori_path(node) << endl;
    }
    
    tampilkan_leaf_kategori(node->firstChild);
    tampilkan_leaf_kategori(node->nextSibling);
}

// Menampilkan kategori berdasarkan level
void tampilkan_kategori_by_level(KategoriNode* node, int targetLevel, int currentLevel = 0) {
    if (node == NULL) return;
    
    if (currentLevel == targetLevel && node->nama != "SEMUA BARANG") {
        string indent = "";
        for (int i = 0; i < currentLevel; i++) indent += "  ";
        cout << indent << "• " << node->nama << " (" << node->kode_kategori << ")" << endl;
    }
    
    tampilkan_kategori_by_level(node->firstChild, targetLevel, currentLevel + 1);
    tampilkan_kategori_by_level(node->nextSibling, targetLevel, currentLevel);
}

// Hapus seluruh tree (untuk cleanup)
void hapus_tree(KategoriNode* node) {
    if (node == NULL) return;
    
    hapus_tree(node->firstChild);
    hapus_tree(node->nextSibling);
    delete node;
}

// Menu manajemen kategori
void menu_kategori() {
    int pilih;
    string nama_induk, nama_baru, nama_cari;
    
    while (true) {
        cout << "\n========================================\n";
        cout << "      MANAJEMEN KATEGORI BARANG\n";
        cout << "========================================\n";
        cout << "1. Tampilkan semua kategori (Tree View)\n";
        cout << "2. Tampilkan kategori level 1 (Utama)\n";
        cout << "3. Tampilkan kategori level 2 (Subkategori)\n";
        cout << "4. Tampilkan kategori terbawah (Leaf)\n";
        cout << "5. Tambah kategori baru\n";
        cout << "6. Cari kategori berdasarkan nama\n";
        cout << "7. Cari kategori berdasarkan kode\n";
        cout << "0. Kembali\n";
        cout << "----------------------------------------\n";
        cout << "Pilih: ";
        cin >> pilih;
        cin.ignore();
        
        if (pilih == 0) break;
        
        switch(pilih) {
            case 1:
                cout << "\n=== STRUKTUR KATEGORI (TREE VIEW) ===\n";
                tampilkan_kategori(rootKategori);
                break;
            case 2:
                cout << "\n=== KATEGORI UTAMA (LEVEL 1) ===\n";
                tampilkan_kategori_by_level(rootKategori, 1);
                break;
            case 3:
                cout << "\n=== SUBKATEGORI (LEVEL 2) ===\n";
                tampilkan_kategori_by_level(rootKategori, 2);
                break;
            case 4:
                cout << "\n=== KATEGORI TERBAWAH (LEAF) ===\n";
                cout << "(Kategori yang bisa dipilih untuk barang)\n\n";
                tampilkan_leaf_kategori(rootKategori);
                break;
            case 5:
                cout << "\n=== TAMBAH KATEGORI BARU ===\n";
                cout << "Masukkan nama kategori induk: ";
                getline(cin, nama_induk);
                cout << "Masukkan nama kategori baru: ";
                getline(cin, nama_baru);
                tambah_kategori(nama_induk, nama_baru);
                break;
            case 6:
                cout << "\nMasukkan nama kategori yang dicari: ";
                getline(cin, nama_cari);
                KategoriNode* hasil;
                hasil = cari_kategori(rootKategori, nama_cari);
                if (hasil) {
                    cout << "\n? Kategori DITEMUKAN!\n";
                    cout << "  Nama    : " << hasil->nama << endl;
                    cout << "  Kode    : " << hasil->kode_kategori << endl;
                    cout << "  Level   : " << hasil->level << endl;
                    cout << "  Path    : " << get_kategori_path(hasil) << endl;
                } else {
                    cout << "\n? Kategori '" << nama_cari << "' tidak ditemukan!\n";
                }
                break;
            case 7:
                cout << "\nMasukkan kode kategori yang dicari: ";
                getline(cin, nama_cari);
                hasil = cari_kategori_by_kode(rootKategori, nama_cari);
                if (hasil) {
                    cout << "\n? Kategori DITEMUKAN!\n";
                    cout << "  Nama    : " << hasil->nama << endl;
                    cout << "  Kode    : " << hasil->kode_kategori << endl;
                    cout << "  Level   : " << hasil->level << endl;
                    cout << "  Path    : " << get_kategori_path(hasil) << endl;
                } else {
                    cout << "\n? Kategori dengan kode '" << nama_cari << "' tidak ditemukan!\n";
                }
                break;
            default:
                cout << "Pilihan tidak valid!\n";
        }
        
        cout << "\nTekan ENTER untuk melanjutkan...";
        cin.get();
    }
}

// Fungsi untuk mendapatkan kategori dari pilihan user (untuk integrasi dengan barang)
string pilih_kategori_untuk_barang() {
    cout << "\n=== PILIH KATEGORI UNTUK BARANG ===\n";
    cout << "Kategori yang tersedia (Leaf):\n";
    tampilkan_leaf_kategori(rootKategori);
    
    string nama_kategori;
    while (true) {
        cout << "\nMasukkan nama kategori (tepat sesuai daftar): ";
        getline(cin, nama_kategori);
        
        KategoriNode* kategori = cari_kategori(rootKategori, nama_kategori);
        if (kategori != NULL && kategori->firstChild == NULL) {
            return nama_kategori;
        }
        cout << "Kategori tidak valid atau bukan kategori leaf! Pilih dari daftar.\n";
    }
}

// Demo / Testing
int main() {
    // Inisialisasi tree kategori
    init_kategori_default();
    
    cout << "========================================\n";
    cout << "   SISTEM MANAJEMEN KATEGORI MINIMARKET\n";
    cout << "========================================\n";
    
    // Tampilkan struktur awal
    cout << "\n=== STRUKTUR KATEGORI DEFAULT ===\n";
    tampilkan_kategori(rootKategori);
    
    // Menu interaktif
    menu_kategori();
    
    // Cleanup (hapus tree untuk menghindari memory leak)
    hapus_tree(rootKategori);
    
    cout << "\nTerima kasih!\n";
    return 0;
}
