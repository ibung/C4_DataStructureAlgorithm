from datasets import load_dataset
import pandas as pd
import os

def download_and_save_dataset(selected_columns=None):
    """
    Mengunduh dataset dari Hugging Face dan menyimpannya sebagai file CSV
    dengan hanya kolom-kolom yang dipilih
    
    Args:
        selected_columns: List berisi nama kolom yang ingin disimpan
                          Jika None, akan menampilkan daftar kolom yang tersedia
    """
    print("Memuat dataset dari 'leminda-ai/s2orc_small'...")
    # Mengunduh dataset
    ds = load_dataset("leminda-ai/s2orc_small")
    
    # Konversi dataset ke DataFrame
    print("Mengkonversi dataset ke DataFrame...")
    df = pd.DataFrame(ds['train'])
    
    # Tampilkan informasi kolom jika kolom belum ditentukan
    if selected_columns is None:
        print("\nKolom yang tersedia dalam dataset:")
        for col in df.columns:
            print(f"- {col}")
        
        # Tunjukkan beberapa baris pertama untuk melihat struktur data
        print("\nContoh 3 baris pertama data:")
        sample_df = df.head(3)
        for col in sample_df.columns:
            print(f"\n{col}:")
            print(sample_df[col].values)
            
        return
    
    # Filter kolom yang dipilih saja
    print(f"Memfilter hanya {len(selected_columns)} kolom yang dipilih...")
    filtered_df = df[selected_columns]
    
    # Simpan ke CSV
    output_file = "s2orc_small_filtered.csv"
    print(f"Menyimpan dataset ke file '{output_file}'...")
    filtered_df.to_csv(output_file, index=False)
    
    print(f"Dataset berhasil disimpan ke '{output_file}'")
    print(f"Jumlah data: {len(filtered_df)} baris")
    print(f"Ukuran file: {os.path.getsize(output_file) / (1024*1024):.2f} MB")

if __name__ == "__main__":
    # Pertama, jalankan tanpa argumen untuk melihat kolom yang tersedia
    print("Mengecek kolom yang tersedia dalam dataset...")
    download_and_save_dataset()
    
    # Setelah melihat daftar kolom, tentukan kolom yang ingin diambil
    # Contoh: kolom yang mungkin Anda butuhkan untuk analisis
    selected_columns = [
        'id',               # ID unik paper
        'title',            # judul paper
        'paperAbstract',  # abstrak paper
        'inCitations'      # jumlah sitasi    
        'fieldsOfStudy',
        'year'
    ]
    
    # Konfirmasi dengan pengguna
    print("\n\nKolom yang akan diambil:")
    for col in selected_columns:
        print(f"- {col}")
    
    proceed = input("\nLanjutkan menyimpan dataset dengan kolom di atas? (y/n): ")
    if proceed.lower() == 'y':
        download_and_save_dataset(selected_columns)
    else:
        # Meminta pengguna untuk memilih kolom sendiri
        print("\nSilakan masukkan nama kolom yang ingin disimpan (pisahkan dengan koma):")
        user_columns = input().strip().split(',')
        user_columns = [col.strip() for col in user_columns]
        download_and_save_dataset(user_columns)