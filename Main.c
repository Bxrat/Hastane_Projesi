#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define Max_Karakter 100
#define MAX_PATIENTS 1000

// ==================== VERİ YAPILARI ====================
typedef struct {
    int id;
    char name[Max_Karakter];
    int health_score;
} Patient;

typedef struct {
    Patient *data;
    int size;
    int cap;
} PatientStore;

// ==================== YARDIMCI FONKSİYONLAR ====================

// Tarih bilgisini al
void get_current_date(int *day, int *month, int *year) {
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    
    *day = local_time->tm_mday;
    *month = local_time->tm_mon + 1;
    *year = local_time->tm_year % 100; // Son 2 basamak
}

// ID oluştur: günay-yıl-öncelik-sıra (12 basamak)
int create_patient_id(int day, int month, int year, int priority, int sequence) {
    int id = day * 100000000 +        // İlk 2 basamak: gün
             month * 1000000 +         // Sonraki 2 basamak: ay
             year * 10000 +            // Sonraki 2 basamak: yıl
             priority * 100 +          // Sonraki 2 basamak: öncelik (01/00)
             sequence;                 // Son 4 basamak: sıra
    return id;
}

// Sağlık puanı hesapla - Poliklinik girişi için
int calculate_health_score_polyclinic() {
    int score = 15;
    int choice;
    
    printf("\n--- Poliklinik Sağlık Puanı Hesaplama ---\n");
    printf("Aşağıdaki işlemlerden hangisi yapıldı? (1-5 arasında seçin, 0 = Bitir)\n");
    printf("1. Tahlil yapıldı (-1 puan)\n");
    printf("2. Tekrar randevu verildi (-2 puan)\n");
    printf("3. Müdahale edildi (-5 puan)\n");
    printf("4. İlaç yazıldı (-1 puan)\n");
    printf("5. Ameliyat randevusu verildi (-6 puan)\n");
    printf("0. Hiçbiri / Bitir\n");
    
    while (1) {
        printf("Seçim yapın (0 ile bitirin): ");
        scanf("%d", &choice);
        getchar(); // Newline karakterini tüket
        
        if (choice == 0) {
            printf("Başlangıç puanı: 15, Son puan: %d\n", score);
            break;
        }
        else if (choice == 1) { score -= 1; printf("Tahlil: -1 puan. Toplam: %d\n", score); }
        else if (choice == 2) { score -= 2; printf("Randevu: -2 puan. Toplam: %d\n", score); }
        else if (choice == 3) { score -= 5; printf("Müdahale: -5 puan. Toplam: %d\n", score); }
        else if (choice == 4) { score -= 1; printf("İlaç: -1 puan. Toplam: %d\n", score); }
        else if (choice == 5) { score -= 6; printf("Ameliyat: -6 puan. Toplam: %d\n", score); }
        else printf("Geçersiz seçim!\n");
    }
    
    return score;
}

// Sağlık puanı hesapla - Acil girişi için
int calculate_health_score_emergency() {
    int code;
    int score;
    
    printf("\n--- Acil Sağlık Puanı Hesaplama ---\n");
    printf("Acil kod türünü seçin:\n");
    printf("1. Kırmızı kod (0 puan)\n");
    printf("2. Sarı kod (8 puan)\n");
    printf("3. Yeşil kod (12 puan)\n");
    printf("Seçim yapın: ");
    scanf("%d", &code);
    getchar(); // Newline karakterini tüket
    
    if (code == 1) score = 0;
    else if (code == 2) score = 8;
    else if (code == 3) score = 12;
    else score = 0;
    
    printf("Sağlık puanı: %d\n", score);
    return score;
}

// Geleneksel Kodlama ile yazılmış fonksiyonlar

// Store'u başlat
PatientStore* initialize_store(int capacity) {
    PatientStore *store = (PatientStore *)malloc(sizeof(PatientStore));
    store->data = (Patient *)malloc(sizeof(Patient) * capacity);
    store->size = 0;
    store->cap = capacity;
    return store;
}

// Hasta ekle (Geleneksel Kodlama)
void add_patient(PatientStore *store, int id, const char *name, int health_score) {
    if (store->size >= store->cap) {
        printf("HATA: Hastane dolu! (Kapasite: %d)\n", store->cap);
        return;
    }
    
    store->data[store->size].id = id;
    strncpy(store->data[store->size].name, name, Max_Karakter - 1);
    store->data[store->size].name[Max_Karakter - 1] = '\0';
    store->data[store->size].health_score = health_score;
    
    store->size++;
    printf("Hasta başarıyla eklendi! ID: %d\n", id);
}

// ID'ye göre sağlık puanını güncelle (Geleneksel Kodlama)
void update_scores_by_id(PatientStore *store, int patient_id, int new_score) {
    int i;
    for (i = 0; i < store->size; i++) {
        if (store->data[i].id == patient_id) {
            store->data[i].health_score = new_score;
            printf("Hasta ID %d sağlık puanı %d olarak güncellendi.\n", patient_id, new_score);
            return;
        }
    }
    printf("HATA: ID %d bulunamadı!\n", patient_id);
}

// ID'ye göre hastayı sil (Geleneksel Kodlama)
void delete_by_id(PatientStore *store, int patient_id) {
    int i, j;
    int found = 0;
    
    for (i = 0; i < store->size; i++) {
        if (store->data[i].id == patient_id) {
            found = 1;
            // Silinen hastanın yerini kaydırma
            for (j = i; j < store->size - 1; j++) {
                store->data[j] = store->data[j + 1];
            }
            store->size--;
            printf("Hasta ID %d başarıyla silindi.\n", patient_id);
            return;
        }
    }
    
    if (!found) {
        printf("HATA: ID %d bulunamadı!\n", patient_id);
    }
}

// Tüm hastaları listele (Geleneksel Kodlama)
void list_patients(PatientStore *store) {
    if (store->size == 0) {
        printf("Sistemde hasta bulunmamaktadır.\n");
        return;
    }
    
    printf("\n========== HASTA LİSTESİ ==========\n");
    printf("%-15s | %-30s | %-15s\n", "ID", "Ad Soyad", "Sağlık Puanı");
    printf("-------------------------------------\n");
    
    int i;
    for (i = 0; i < store->size; i++) {
        printf("%-15d | %-30s | %-15d\n", 
               store->data[i].id, 
               store->data[i].name, 
               store->data[i].health_score);
    }
    printf("====================================\n");
    printf("Toplam hasta sayısı: %d / %d\n\n", store->size, store->cap);
}

// ==================== VİBE CODING - ADVANCED OPERATIONS ====================

// Ada göre ara (Vibe Coding)
void search_by_name(PatientStore *store, const char *search_name) {
    printf("\n--- Ada Göre Arama Sonuçları ---\n");
    
    int found = 0;
    int i;
    
    for (i = 0; i < store->size; i++) {
        if (strstr(store->data[i].name, search_name) != NULL) {
            printf("ID: %d | Ad: %s | Sağlık Puanı: %d\n", 
                   store->data[i].id, 
                   store->data[i].name, 
                   store->data[i].health_score);
            found = 1;
        }
    }
    
    if (!found) {
        printf("'%s' içeren hasta bulunamadı.\n", search_name);
    }
    printf("\n");
}

// Ortalama puana göre sıralama (Vibe Coding - Bubble Sort)
void sort_by_average(PatientStore *store) {
    if (store->size <= 1) {
        printf("Sıralama için yeterli hasta yok.\n");
        return;
    }
    
    // Temp store oluştur (sıralamayı orijinal veriye etki etmemesi için)
    Patient *sorted = (Patient *)malloc(sizeof(Patient) * store->size);
    int i, j;
    
    // Verileri kopyala
    for (i = 0; i < store->size; i++) {
        sorted[i] = store->data[i];
    }
    
    // Bubble Sort - Azalan sıra (yüksek puan önce)
    for (i = 0; i < store->size - 1; i++) {
        for (j = 0; j < store->size - i - 1; j++) {
            if (sorted[j].health_score < sorted[j + 1].health_score) {
                // Swap
                Patient temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }
    
    printf("\n========== SAĞLIK PUANINA GÖRE SIRALI HASTALAR ==========\n");
    printf("%-15s | %-30s | %-15s\n", "ID", "Ad Soyad", "Sağlık Puanı");
    printf("----------------------------------------------------------\n");
    
    for (i = 0; i < store->size; i++) {
        printf("%-15d | %-30s | %-15d\n", 
               sorted[i].id, 
               sorted[i].name, 
               sorted[i].health_score);
    }
    printf("==========================================================\n\n");
    
    free(sorted);
}

// ==================== BELLEKİ TEMİZLE ====================

void cleanup_store(PatientStore *store) {
    if (store != NULL) {
        if (store->data != NULL) {
            free(store->data);
        }
        free(store);
    }
}

// ==================== ARAYÜZ (MENU) ====================

void display_menu() {
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║     HASTANE HASTA YÖNETİM SİSTEMİ    ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("1. Yeni Hasta Ekle\n");
    printf("2. Hastayı Sil (ID ile)\n");
    printf("3. Hastayı Güncelle (ID ile)\n");
    printf("4. Tüm Hastaları Listele\n");
    printf("5. Ada Göre Ara\n");
    printf("6. Sağlık Puanına Göre Sırala\n");
    printf("7. Çıkış\n");
    printf("Seçim yapın: ");
}

// ==================== MAIN FONKSİYON ====================

int main() {
    setbuf(stdout, NULL);
    
    // Hastane deposu başlat
    PatientStore *hospital = initialize_store(MAX_PATIENTS);
    
    int day, month, year;
    get_current_date(&day, &month, &year);
    
    int choice;
    int patient_count = 0;  // Sistem tarafından kaydedilen hasta sayısı
    
    printf("Hastane Sistemi Başlatıldı!\n");
    printf("Tarih: %02d.%02d.20%02d\n\n", day, month, year);
    
    while (1) {
        display_menu();
        scanf("%d", &choice);
        getchar(); // Newline karakterini tüket
        
        if (choice == 1) {
            // Yeni Hasta Ekle
            printf("\n--- YENİ HASTA KAYDÜ ---\n");
            
            char name[Max_Karakter];
            int priority;
            int entry_type;
            int health_score;
            
            printf("Hastanın adı ve soyadı: ");
            fgets(name, Max_Karakter, stdin);
            name[strcspn(name, "\n")] = '\0'; // Newline kaldır
            
            printf("Giriş türü (1=Poliklinik, 2=Acil): ");
            scanf("%d", &entry_type);
            
            printf("Hasta öncelikli mi? (1=Evet/Öncelikli, 0=Hayır): ");
            scanf("%d", &priority);
            
            getchar(); // Newline karakterini tüket
            
            // Sağlık puanı hesapla
            if (entry_type == 1) {
                health_score = calculate_health_score_polyclinic();
            } else if (entry_type == 2) {
                health_score = calculate_health_score_emergency();
            } else {
                health_score = 15;
            }
            
            // Hastanın sıra numarası artır
            patient_count++;
            
            // ID oluştur
            int patient_id = create_patient_id(day, month, year, priority, patient_count);
            
            // Hasta ekle
            add_patient(hospital, patient_id, name, health_score);
            
        } else if (choice == 2) {
            // Hastayı Sil
            printf("\n--- HASTA SİLME ---\n");
            int delete_id;
            printf("Silinecek hastanın ID'si: ");
            scanf("%d", &delete_id);
            delete_by_id(hospital, delete_id);
            
        } else if (choice == 3) {
            // Hastayı Güncelle
            printf("\n--- HASTA GÜNCELLEME ---\n");
            int update_id, new_score;
            printf("Güncellenecek hastanın ID'si: ");
            scanf("%d", &update_id);
            printf("Yeni sağlık puanı: ");
            scanf("%d", &new_score);
            update_scores_by_id(hospital, update_id, new_score);
            
        } else if (choice == 4) {
            // Tüm Hastaları Listele
            list_patients(hospital);
            
        } else if (choice == 5) {
            // Ada Göre Ara
            printf("\n--- ADA GÖRE ARAMA ---\n");
            char search_name[Max_Karakter];
            printf("Aranacak isim: ");
            fgets(search_name, Max_Karakter, stdin);
            search_name[strcspn(search_name, "\n")] = '\0';
            search_by_name(hospital, search_name);
            
        } else if (choice == 6) {
            // Sağlık Puanına Göre Sırala
            sort_by_average(hospital);
            
        } else if (choice == 7) {
            // Çıkış
            printf("\nSistem kapatılıyor... Bellek temizleniyor...\n");
            cleanup_store(hospital);
            printf("Hoşça kalın!\n");
            break;
            
        } else {
            printf("Geçersiz seçim! Lütfen 1-7 arasında bir seçim yapın.\n");
        }
    }
    
    return 0;
}
