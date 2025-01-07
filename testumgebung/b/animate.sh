#!/bin/bash

# Initialisiere Zähler
counter=1

# Erstelle eine Funktion zum Generieren von Dateienamen mit führenden Nullen
pad_number() {
  printf "%03d" $1
}

# Schleife durch alle Bilddateien im aktuellen Verzeichnis
for image in *.png *.jpg *.jpeg; do
  # Prüfe, ob Dateien existieren
  if [ ! -e "$image" ]; then
    echo "Keine Bilddateien gefunden."
    exit 1
  fi

  echo "Bearbeite: $image"

  # Starte Pixelsortierer
  time ./Pixelsortierer 2 "$image"

  # Generiere Dateinamen mit Zähler
  padded_counter=$(pad_number $counter)
  output_file="a${padded_counter}.mp4"

  # Versuche, Video mit ffmpeg zu erstellen
  if ffmpeg -framerate 60 -i outanim_%d.png -c:v libx264 -pix_fmt yuv420p "$output_file"; then
    echo "Erfolgreich verarbeitet: $output_file"
  else
    echo "Fehler mit ffmpeg, wende Korrektur an..."
    if ffmpeg -framerate 60 -i outanim_%d.png -vf "pad=ceil(iw/2)*2:ceil(ih/2)*2" -c:v libx264 -pix_fmt yuv420p "$output_file"; then
      echo "Erfolgreich verarbeitet (mit Korrektur): $output_file"
    else
      echo "Fehler: Video konnte nicht erstellt werden."
      exit 1
    fi
  fi

  # Zähler erhöhen
  counter=$((counter + 1))

done

echo "Alle Bilder erfolgreich verarbeitet."
