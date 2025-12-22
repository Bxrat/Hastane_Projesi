#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define Max_Karakter 100
#define MAX_PATIENTS 1000
#define LOG_FILE "hospital_log.txt"
#define DEFAULT_PATIENTS_FILE "hastalar_30.csv"

// ==================== VERİ YAPILARI ====================
typedef struct {
    long id;
    char name[Max_Karakter];
    int health_score;
} Patient;

typedef struct {
    Patient *data;
    int size;
    int cap;
} PatientStore;

// ==================== YARDIMCI FONKSİYONLAR ====================

// Log sistemi
void log_action(const char *action) {
    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL) {
        printf("HATA: Log dosyası açılamadı!\n");
        return;
    }
    
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    
    fprintf(log_file, "[%02d.%02d.%04d %02d:%02d:%02d] %s\n",
            local_time->tm_mday,
            local_time->tm_mon + 1,
            local_time->tm_year + 1900,
            local_time->tm_hour,
            local_time->tm_min,
            local_time->tm_sec,
            action);
    
    fclose(log_file);
}

// Tarih bilgisini al
void get_current_date(int *day, int *month, int *year) {
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    
    *day = local_time->tm_mday;
    *month = local_time->tm_mon + 1;
    *year = local_time->tm_year % 100; // Son 2 basamak
}

// ID oluştur: günay-yıl-öncelik-sıra (12 basamak)
long create_patient_id(int day, int month, int year, int priority, int sequence) {
    long id = (long)day * 1000000000LL +      // GG
              (long)month * 10000000LL +       // MM
              (long)year * 100000LL +          // YY
              (long)priority * 10000LL +       // P
              (long)sequence;                  // SSSS
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
void add_patient(PatientStore *store, long id, const char *name, int health_score) {
    if (store->size >= store->cap) {
        printf("HATA: Hastane dolu! (Kapasite: %d)\n", store->cap);
        return;
    }
    
    store->data[store->size].id = id;
    strncpy(store->data[store->size].name, name, Max_Karakter - 1);
    store->data[store->size].name[Max_Karakter - 1] = '\0';
    store->data[store->size].health_score = health_score;
    
    store->size++;
    printf("Hasta başarıyla eklendi! ID: %ld\n", id);
}

// ID'ye göre sağlık puanını güncelle (Geleneksel Kodlama)
void update_scores_by_id(PatientStore *store, long patient_id, int new_score) {
    int i;
    for (i = 0; i < store->size; i++) {
        if (store->data[i].id == patient_id) {
            store->data[i].health_score = new_score;
            printf("Hasta ID %ld sağlık puanı %d olarak güncellendi.\n", patient_id, new_score);
            return;
        }
    }
    printf("HATA: ID %ld bulunamadı!\n", patient_id);
}

// ID'ye göre hastayı sil (Geleneksel Kodlama)
void delete_by_id(PatientStore *store, long patient_id) {
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
            printf("Hasta ID %ld başarıyla silindi.\n", patient_id);
            return;
        }
    }
    
    if (!found) {
        printf("HATA: ID %ld bulunamadı!\n", patient_id);
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
        printf("%-15ld | %-30s | %-15d\n", 
               store->data[i].id, 
               store->data[i].name, 
               store->data[i].health_score);
    }
    printf("====================================\n");
    printf("Toplam hasta sayısı: %d / %d\n\n", store->size, store->cap);
}

// ==================== VİBE CODING - ADVANCED OPERATIONS ====================

// CSV formatında kaydetme
int save_patients_to_csv(const PatientStore *ps, const char *filename) {
    if (ps == NULL || filename == NULL) {
        log_action("HATA: CSV kaydetme - Geçersiz parametre");
        return 0;
    }
    
    FILE *csv_file = fopen(filename, "w");
    if (csv_file == NULL) {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "HATA: CSV dosyası açılamadı - %s", filename);
        log_action(error_msg);
        printf("HATA: %s dosyası açılamadı!\n", filename);
        return 0;
    }
    
    // CSV Header
    fprintf(csv_file, "ID,Ad Soyad,Saglik Puani\n");
    
    int i;
    for (i = 0; i < ps->size; i++) {
        fprintf(csv_file, "%ld,%s,%d\n",
                ps->data[i].id,
                ps->data[i].name,
                ps->data[i].health_score);
    }
    
    fclose(csv_file);
    
    char success_msg[256];
    snprintf(success_msg, sizeof(success_msg), "CSV kaydetme basarili - %s (%d hasta kaydedildi)", filename, ps->size);
    log_action(success_msg);
    printf("Hastalar CSV formatında başarıyla '%s' dosyasına kaydedildi! (%d hasta)\n", filename, ps->size);
    
    return 1;
}

// CSV formatından yükleme
int load_patients_from_csv(PatientStore *ps, const char *filename) {
    if (ps == NULL || filename == NULL) {
        log_action("HATA: CSV yukleme - Geçersiz parametre");
        return 0;
    }
    
    FILE *csv_file = fopen(filename, "r");
    if (csv_file == NULL) {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "HATA: CSV dosyası bulunamadı - %s", filename);
        log_action(error_msg);
        printf("HATA: %s dosyası bulunamadı!\n", filename);
        return 0;
    }
    
    char line[512];
    int loaded_count = 0;
    
    // Header satırını atla
    fgets(line, sizeof(line), csv_file);
    
    // Verileri oku
    while (fgets(line, sizeof(line), csv_file) != NULL && ps->size < ps->cap) {
        long id;
        int health_score;
        char name[Max_Karakter];
        
        // CSV'den verileri parse et
        if (sscanf(line, "%ld,%99[^,],%d", &id, name, &health_score) == 3) {
            add_patient(ps, id, name, health_score);
            loaded_count++;
        }
    }
    
    fclose(csv_file);
    
    char success_msg[256];
    snprintf(success_msg, sizeof(success_msg), "CSV yukleme basarili - %s (%d hasta yuklendi)", filename, loaded_count);
    log_action(success_msg);
    printf("Hastalar CSV dosyasından başarıyla yüklendi! (%d hasta)\n", loaded_count);
    
    return loaded_count;
}

// JSON formatında kaydetme
int save_patients_to_json(const PatientStore *ps, const char *filename) {
    if (ps == NULL || filename == NULL) {
        log_action("HATA: JSON kaydetme - Geçersiz parametre");
        return 0;
    }
    
    FILE *json_file = fopen(filename, "w");
    if (json_file == NULL) {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "HATA: JSON dosyası açılamadı - %s", filename);
        log_action(error_msg);
        printf("HATA: %s dosyası açılamadı!\n", filename);
        return 0;
    }
    
    // JSON başlangıcı
    fprintf(json_file, "{\n");
    fprintf(json_file, "  \"hastalar\": [\n");
    
    int i;
    for (i = 0; i < ps->size; i++) {
        fprintf(json_file, "    {\n");
        fprintf(json_file, "      \"id\": %ld,\n", ps->data[i].id);
        fprintf(json_file, "      \"ad_soyad\": \"%s\",\n", ps->data[i].name);
        fprintf(json_file, "      \"saglik_puani\": %d\n", ps->data[i].health_score);
        
        if (i < ps->size - 1) {
            fprintf(json_file, "    },\n");
        } else {
            fprintf(json_file, "    }\n");
        }
    }
    
    fprintf(json_file, "  ],\n");
    fprintf(json_file, "  \"toplam_hasta\": %d\n", ps->size);
    fprintf(json_file, "}\n");
    
    fclose(json_file);
    
    char success_msg[256];
    snprintf(success_msg, sizeof(success_msg), "JSON kaydetme basarili - %s (%d hasta kaydedildi)", filename, ps->size);
    log_action(success_msg);
    printf("Hastalar JSON formatında başarıyla '%s' dosyasına kaydedildi! (%d hasta)\n", filename, ps->size);
    
    return 1;
}

// JSON formatından yükleme
int load_patients_from_json(PatientStore *ps, const char *filename) {
    if (ps == NULL || filename == NULL) {
        log_action("HATA: JSON yukleme - Geçersiz parametre");
        return 0;
    }
    
    FILE *json_file = fopen(filename, "r");
    if (json_file == NULL) {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "HATA: JSON dosyası bulunamadı - %s", filename);
        log_action(error_msg);
        printf("HATA: %s dosyası bulunamadı!\n", filename);
        return 0;
    }
    
    char line[512];
    int loaded_count = 0;
    long id = 0;
    int health_score = 0;
    char name[Max_Karakter] = {0};
    
    while (fgets(line, sizeof(line), json_file) != NULL) {
        // "id" alanını ara
        if (strstr(line, "\"id\":") != NULL) {
            sscanf(line, " %*[^:]: %ld", &id);
        }
        // "ad_soyad" alanını ara
        else if (strstr(line, "\"ad_soyad\":") != NULL) {
            // İsmi parse et: "ad_soyad": "İsim"
            char *start = strchr(line, ':');
            if (start) {
                start = strchr(start, '"');
                if (start) {
                    start++;
                    char *end = strchr(start, '"');
                    if (end) {
                        int len = end - start;
                        if (len > 0 && len < Max_Karakter) {
                            strncpy(name, start, len);
                            name[len] = '\0';
                        }
                    }
                }
            }
        }
        // "saglik_puani" alanını ara
        else if (strstr(line, "\"saglik_puani\":") != NULL) {
            sscanf(line, " %*[^:]: %d", &health_score);
            
            // Hasta bilgilerini kaydet
            if (id > 0 && strlen(name) > 0 && ps->size < ps->cap) {
                add_patient(ps, id, name, health_score);
                loaded_count++;
                id = 0;
                health_score = 0;
                memset(name, 0, sizeof(name));
            }
        }
    }
    
    fclose(json_file);
    
    char success_msg[256];
    snprintf(success_msg, sizeof(success_msg), "JSON yukleme basarili - %s (%d hasta yuklendi)", filename, loaded_count);
    log_action(success_msg);
    printf("Hastalar JSON dosyasından başarıyla yüklendi! (%d hasta)\n", loaded_count);
    
    return loaded_count;
}

// Ada göre ara (Vibe Coding)
void search_by_name(PatientStore *store, const char *search_name) {
    printf("\n--- Ada Göre Arama Sonuçları ---\n");
    
    int found = 0;
    int i;
    
    for (i = 0; i < store->size; i++) {
        if (strstr(store->data[i].name, search_name) != NULL) {
            printf("ID: %ld | Ad: %s | Sağlık Puanı: %d\n", 
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
        printf("%-15ld | %-30s | %-15d\n", 
               sorted[i].id, 
               sorted[i].name, 
               sorted[i].health_score);
    }
    printf("==========================================================\n\n");
    
    free(sorted);
}

// ==================== DOSYA TABANLI İŞLEMLER ====================

// Dosyaya yeni hasta ekle
void add_patient_to_file(const char *filename, long id, const char *name, int health_score) {
    FILE *csv_file = fopen(filename, "a");
    if (csv_file == NULL) {
        printf("HATA: %s dosyası açılamadı!\n", filename);
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "HATA: Hasta ekleme - dosya acilamadi %s", filename);
        log_action(error_msg);
        return;
    }
    
    fprintf(csv_file, "%ld,%s,%d\n", id, name, health_score);
    fclose(csv_file);
    
    printf("Hasta başarıyla dosyaya eklendi! ID: %ld\n", id);
    
    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "Hasta dosyaya eklendi - ID: %ld, Ad: %s, Saglik Puani: %d", id, name, health_score);
    log_action(log_msg);
}

// Dosyadan hastayı sil (ID ile)
void delete_patient_from_file(const char *filename, long patient_id) {
    FILE *csv_file = fopen(filename, "r");
    if (csv_file == NULL) {
        printf("HATA: %s dosyası bulunamadı!\n", filename);
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "HATA: Hasta silme - dosya bulunamadi %s", filename);
        log_action(error_msg);
        return;
    }
    
    FILE *temp_file = fopen("temp_patients.csv", "w");
    if (temp_file == NULL) {
        printf("HATA: Geçici dosya oluşturulamadı!\n");
        fclose(csv_file);
        return;
    }
    
    char line[512];
    int found = 0;
    int line_count = 0;
    
    // Header satırını yaz
    fgets(line, sizeof(line), csv_file);
    fprintf(temp_file, "%s", line);
    
    // Verileri oku ve sil
    while (fgets(line, sizeof(line), csv_file) != NULL) {
        long id;
        int health_score;
        char name[Max_Karakter];
        
        if (sscanf(line, "%ld,%99[^,],%d", &id, name, &health_score) == 3) {
            if (id == patient_id) {
                found = 1;
                printf("Hasta ID %ld başarıyla silindi.\n", patient_id);
            } else {
                fprintf(temp_file, "%s", line);
            }
        }
    }
    
    fclose(csv_file);
    fclose(temp_file);
    
    // Dosyaları değiştir
    remove(filename);
    rename("temp_patients.csv", filename);
    
    if (found) {
        char log_msg[256];
        snprintf(log_msg, sizeof(log_msg), "Hasta dosyadan silindi - ID: %ld", patient_id);
        log_action(log_msg);
    } else {
        printf("HATA: ID %ld bulunamadı!\n", patient_id);
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "HATA: Hasta silme - ID bulunamadi %ld", patient_id);
        log_action(error_msg);
    }
}

// Dosyada hastayı güncelle
void update_patient_in_file(const char *filename, long patient_id, int new_score) {
    FILE *csv_file = fopen(filename, "r");
    if (csv_file == NULL) {
        printf("HATA: %s dosyası bulunamadı!\n", filename);
        return;
    }
    
    FILE *temp_file = fopen("temp_patients.csv", "w");
    if (temp_file == NULL) {
        printf("HATA: Geçici dosya oluşturulamadı!\n");
        fclose(csv_file);
        return;
    }
    
    char line[512];
    int found = 0;
    
    // Header satırını yaz
    fgets(line, sizeof(line), csv_file);
    fprintf(temp_file, "%s", line);
    
    // Verileri oku ve güncelle
    while (fgets(line, sizeof(line), csv_file) != NULL) {
        long id;
        int health_score;
        char name[Max_Karakter];
        
        if (sscanf(line, "%ld,%99[^,],%d", &id, name, &health_score) == 3) {
            if (id == patient_id) {
                found = 1;
                fprintf(temp_file, "%ld,%s,%d\n", id, name, new_score);
                printf("Hasta ID %ld sağlık puanı %d olarak güncellendi.\n", patient_id, new_score);
            } else {
                fprintf(temp_file, "%s", line);
            }
        }
    }
    
    fclose(csv_file);
    fclose(temp_file);
    
    // Dosyaları değiştir
    remove(filename);
    rename("temp_patients.csv", filename);
    
    if (found) {
        char log_msg[256];
        snprintf(log_msg, sizeof(log_msg), "Hasta dosyada guncellendi - ID: %ld, Yeni Saglik Puani: %d", patient_id, new_score);
        log_action(log_msg);
    } else {
        printf("HATA: ID %ld bulunamadı!\n", patient_id);
    }
}

// Dosyadan ada göre ara
void search_patient_by_name_in_file(const char *filename, const char *search_name) {
    FILE *csv_file = fopen(filename, "r");
    if (csv_file == NULL) {
        printf("HATA: %s dosyası bulunamadı!\n", filename);
        return;
    }
    
    printf("\n--- Ada Göre Arama Sonuçları (Dosyadan) ---\n");
    
    char line[512];
    int found = 0;
    
    // Header satırını atla
    fgets(line, sizeof(line), csv_file);
    
    // Verileri oku ve ara
    while (fgets(line, sizeof(line), csv_file) != NULL) {
        long id;
        int health_score;
        char name[Max_Karakter];
        
        if (sscanf(line, "%ld,%99[^,],%d", &id, name, &health_score) == 3) {
            if (strstr(name, search_name) != NULL) {
                printf("ID: %ld | Ad: %s | Sağlık Puanı: %d\n", id, name, health_score);
                found = 1;
            }
        }
    }
    
    fclose(csv_file);
    
    if (!found) {
        printf("'%s' içeren hasta bulunamadı.\n", search_name);
    }
    printf("\n");
    
    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "Dosyada ada gore arama yapildi - Aranan: %s", search_name);
    log_action(log_msg);
}

// Dosyadan hastaları sağlık puanına göre sırala
void sort_patients_in_file(const char *filename) {
    FILE *csv_file = fopen(filename, "r");
    if (csv_file == NULL) {
        printf("HATA: %s dosyası bulunamadı!\n", filename);
        return;
    }
    
    // Tüm hastaları bellekte oku
    Patient *patients = (Patient *)malloc(sizeof(Patient) * MAX_PATIENTS);
    int count = 0;
    char line[512];
    
    // Header satırını atla
    fgets(line, sizeof(line), csv_file);
    
    // Verileri oku
    while (fgets(line, sizeof(line), csv_file) != NULL && count < MAX_PATIENTS) {
        long id;
        int health_score;
        char name[Max_Karakter];
        
        if (sscanf(line, "%ld,%99[^,],%d", &id, name, &health_score) == 3) {
            patients[count].id = id;
            strcpy(patients[count].name, name);
            patients[count].health_score = health_score;
            count++;
        }
    }
    
    fclose(csv_file);
    
    // Bubble Sort - Azalan sıra
    int i, j;
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            if (patients[j].health_score < patients[j + 1].health_score) {
                Patient temp = patients[j];
                patients[j] = patients[j + 1];
                patients[j + 1] = temp;
            }
        }
    }
    
    printf("\n========== SAĞLIK PUANINA GÖRE SIRALI HASTALAR ==========\n");
    printf("%-15s | %-30s | %-15s\n", "ID", "Ad Soyad", "Sağlık Puanı");
    printf("----------------------------------------------------------\n");
    
    for (i = 0; i < count; i++) {
        printf("%-15ld | %-30s | %-15d\n", 
               patients[i].id, 
               patients[i].name, 
               patients[i].health_score);
    }
    printf("==========================================================\n\n");
    
    free(patients);
    
    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "Dosyadaki hastalar saglik puanina gore siralandi - %d hasta", count);
    log_action(log_msg);
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

// ==================== DOSYADAN OKUMA VE LISTELEME ====================

// Dosyadan hastaları oku ve listele (CSV formatı)
void list_patients_from_csv_file(const char *filename) {
    FILE *csv_file = fopen(filename, "r");
    if (csv_file == NULL) {
        printf("HATA: %s dosyası bulunamadı!\n", filename);
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "HATA: CSV listeleme dosyasi bulunamadi - %s", filename);
        log_action(error_msg);
        return;
    }
    
    printf("\n========== HASTA LİSTESİ (CSV'DEN) ==========\n");
    printf("%-15s | %-30s | %-15s\n", "ID", "Ad Soyad", "Sağlık Puanı");
    printf("-------------------------------------\n");
    
    char line[512];
    int patient_count = 0;
    
    // Header satırını atla
    fgets(line, sizeof(line), csv_file);
    
    // Verileri oku ve listele
    while (fgets(line, sizeof(line), csv_file) != NULL) {
        long id;
        int health_score;
        char name[Max_Karakter];
        
        // CSV'den verileri parse et
        if (sscanf(line, "%ld,%99[^,],%d", &id, name, &health_score) == 3) {
            printf("%-15ld | %-30s | %-15d\n", id, name, health_score);
            patient_count++;
        }
    }
    
    printf("====================================\n");
    printf("Toplam hasta sayısı: %d\n\n", patient_count);
    
    fclose(csv_file);
    
    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "CSV dosyasindan hastalar listelendi - %s (%d hasta)", filename, patient_count);
    log_action(log_msg);
}

// Dosyadan hastaları oku ve listele (JSON formatı)
void list_patients_from_json_file(const char *filename) {
    FILE *json_file = fopen(filename, "r");
    if (json_file == NULL) {
        printf("HATA: %s dosyası bulunamadı!\n", filename);
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "HATA: JSON listeleme dosyasi bulunamadi - %s", filename);
        log_action(error_msg);
        return;
    }
    
    printf("\n========== HASTA LİSTESİ (JSON'DAN) ==========\n");
    printf("%-15s | %-30s | %-15s\n", "ID", "Ad Soyad", "Sağlık Puanı");
    printf("-------------------------------------\n");
    
    char line[512];
    int patient_count = 0;
    long id = 0;
    int health_score = 0;
    char name[Max_Karakter] = {0};
    
    while (fgets(line, sizeof(line), json_file) != NULL) {
        // "id" alanını ara
        if (strstr(line, "\"id\":") != NULL) {
            sscanf(line, " %*[^:]: %ld", &id);
        }
        // "ad_soyad" alanını ara
        else if (strstr(line, "\"ad_soyad\":") != NULL) {
            // İsmi parse et: "ad_soyad": "İsim"
            char *start = strchr(line, ':');
            if (start) {
                start = strchr(start, '"');
                if (start) {
                    start++;
                    char *end = strchr(start, '"');
                    if (end) {
                        int len = end - start;
                        if (len > 0 && len < Max_Karakter) {
                            strncpy(name, start, len);
                            name[len] = '\0';
                        }
                    }
                }
            }
        }
        // "saglik_puani" alanını ara
        else if (strstr(line, "\"saglik_puani\":") != NULL) {
            sscanf(line, " %*[^:]: %d", &health_score);
            
            // Hasta bilgilerini listele
            if (id > 0 && strlen(name) > 0) {
                printf("%-15ld | %-30s | %-15d\n", id, name, health_score);
                patient_count++;
                id = 0;
                health_score = 0;
                memset(name, 0, sizeof(name));
            }
        }
    }
    
    printf("====================================\n");
    printf("Toplam hasta sayısı: %d\n\n", patient_count);
    
    fclose(json_file);
    
    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "JSON dosyasindan hastalar listelendi - %s (%d hasta)", filename, patient_count);
    log_action(log_msg);
}

// ==================== ARAYÜZ (MENU) ====================

void display_menu() {
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║     HASTANE HASTA YÖNETİM SİSTEMİ    ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("1. Yeni Hasta Ekle (Dosyaya)\n");
    printf("2. Hastayı Sil (ID ile - Dosyadan)\n");
    printf("3. Hastayı Güncelle (ID ile - Dosyaya)\n");
    printf("4. Ada Göre Ara (Dosyadan)\n");
    printf("5. Sağlık Puanına Göre Sırala (Dosyadan)\n");
    printf("6. Hastaları CSV Dosyasından Yükle\n");
    printf("7. Hastaları JSON Dosyasından Yükle\n");
    printf("8. CSV Dosyasından Listele (Dosyadan)\n");
    printf("9. JSON Dosyasından Listele (Dosyadan)\n");
    printf("10. Log Dosyasını Görüntüle\n");
    printf("11. Çıkış\n");
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
    log_action("Sistem baslatildi");
    
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
            long patient_id = create_patient_id(day, month, year, priority, patient_count);
            
            // Hasta ekle
            add_patient(hospital, patient_id, name, health_score);
            
            // Log'a kaydet
            char log_msg[256];
            snprintf(log_msg, sizeof(log_msg), "Yeni hasta eklendi - ID: %ld, Ad: %s, Saglik Puani: %d", patient_id, name, health_score);
            log_action(log_msg);
            
        } else if (choice == 2) {
            // Hastayı Sil
            printf("\n--- HASTA SİLME ---\n");
            long delete_id;
            printf("Silinecek hastanın ID'si: ");
            scanf("%ld", &delete_id);
            delete_by_id(hospital, delete_id);
            
            char log_msg[256];
            snprintf(log_msg, sizeof(log_msg), "Hasta silme istegi - ID: %ld", delete_id);
            log_action(log_msg);
            
        } else if (choice == 3) {
            // Hastayı Güncelle
            printf("\n--- HASTA GÜNCELLEME ---\n");
            long update_id;
            int new_score;
            printf("Güncellenecek hastanın ID'si: ");
            scanf("%ld", &update_id);
            printf("Yeni sağlık puanı: ");
            scanf("%d", &new_score);
            update_scores_by_id(hospital, update_id, new_score);
            
            char log_msg[256];
            snprintf(log_msg, sizeof(log_msg), "Hasta guncelleme - ID: %ld, Yeni Saglik Puani: %d", update_id, new_score);
            log_action(log_msg);
            
        } else if (choice == 4) {
            // Tüm Hastaları Listele
            list_patients(hospital);
            log_action("Tum hastalar listelendi");
            
        } else if (choice == 5) {
            // Ada Göre Ara
            printf("\n--- ADA GÖRE ARAMA ---\n");
            char search_name[Max_Karakter];
            printf("Aranacak isim: ");
            fgets(search_name, Max_Karakter, stdin);
            search_name[strcspn(search_name, "\n")] = '\0';
            search_by_name(hospital, search_name);
            
            char log_msg[256];
            snprintf(log_msg, sizeof(log_msg), "Ada gore arama yapildi - Aranan: %s", search_name);
            log_action(log_msg);
            
        } else if (choice == 6) {
            // Sağlık Puanına Göre Sırala
            sort_by_average(hospital);
            log_action("Hastalar saglik puanina gore siralandi");
            
        } else if (choice == 7) {
            // Hastaları CSV Olarak Kaydet
            printf("\n--- HASTALARI CSV OLARAK KAYDET ---\n");
            char csv_filename[Max_Karakter];
            printf("CSV dosyasının adı (örn: hastalar.csv): ");
            fgets(csv_filename, Max_Karakter, stdin);
            csv_filename[strcspn(csv_filename, "\n")] = '\0';
            save_patients_to_csv(hospital, csv_filename);
            
        } else if (choice == 8) {
            // Hastaları CSV Dosyasından Yükle
            printf("\n--- HASTALARI CSV DOSYASINDAN YÜKLE ---\n");
            char csv_filename[Max_Karakter];
            printf("CSV dosyasının adı: ");
            fgets(csv_filename, Max_Karakter, stdin);
            csv_filename[strcspn(csv_filename, "\n")] = '\0';
            load_patients_from_csv(hospital, csv_filename);
            
        } else if (choice == 9) {
            // Hastaları JSON Olarak Kaydet
            printf("\n--- HASTALARI JSON OLARAK KAYDET ---\n");
            char json_filename[Max_Karakter];
            printf("JSON dosyasının adı (örn: hastalar.json): ");
            fgets(json_filename, Max_Karakter, stdin);
            json_filename[strcspn(json_filename, "\n")] = '\0';
            save_patients_to_json(hospital, json_filename);
            
        } else if (choice == 10) {
            // Hastaları JSON Dosyasından Yükle
            printf("\n--- HASTALARI JSON DOSYASINDAN YÜKLE ---\n");
            char json_filename[Max_Karakter];
            printf("JSON dosyasının adı: ");
            fgets(json_filename, Max_Karakter, stdin);
            json_filename[strcspn(json_filename, "\n")] = '\0';
            load_patients_from_json(hospital, json_filename);
            
        } else if (choice == 11) {
            // CSV Dosyasından Listele (Dosyadan)
            printf("\n--- CSV DOSYASINDAN LISTELE ---\n");
            char csv_filename[Max_Karakter];
            printf("CSV dosyasının adı: ");
            fgets(csv_filename, Max_Karakter, stdin);
            csv_filename[strcspn(csv_filename, "\n")] = '\0';
            list_patients_from_csv_file(csv_filename);
            
        } else if (choice == 12) {
            // JSON Dosyasından Listele (Dosyadan)
            printf("\n--- JSON DOSYASINDAN LISTELE ---\n");
            char json_filename[Max_Karakter];
            printf("JSON dosyasının adı: ");
            fgets(json_filename, Max_Karakter, stdin);
            json_filename[strcspn(json_filename, "\n")] = '\0';
            list_patients_from_json_file(json_filename);
            
        } else if (choice == 13) {
            // Log Dosyasını Görüntüle
            printf("\n--- LOG DOSYASI ---\n");
            FILE *log_file = fopen(LOG_FILE, "r");
            if (log_file == NULL) {
                printf("HATA: Log dosyası bulunamadı!\n");
                log_action("HATA: Log dosyasi goruntulenmeye calisildi ancak bulunamadi");
            } else {
                char line[512];
                while (fgets(line, sizeof(line), log_file) != NULL) {
                    printf("%s", line);
                }
                fclose(log_file);
                printf("\n");
                log_action("Log dosyasi gorunturlendi");
            }
            
        } else if (choice == 14) {
            // Çıkış
            printf("\nSistem kapatılıyor... Bellek temizleniyor...\n");
            log_action("Sistem kapatildi");
            cleanup_store(hospital);
            printf("Hoşça kalın!\n");
            break;
            
        } else {
            printf("Geçersiz seçim! Lütfen 1-14 arasında bir seçim yapın.\n");
            log_action("Gecersiz menu secimi yapildi");
        }
    }
    
    return 0;
}
