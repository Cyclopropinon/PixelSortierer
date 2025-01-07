#!/bin/bash

# Initialisiere Zähler
counter=1

# Überprüfe, ob mindestens eine Bilddatei existiert
shopt -s nullglob
images=( *.png *.jpg *.jpeg )
if [ ${#images[@]} -eq 0 ]; then
  echo "Keine Bilddateien gefunden."
  exit 1
fi

# Schleife durch alle Bilddateien im aktuellen Verzeichnis
for image in "${images[@]}"; do
  # Prüfe, ob Dateien existieren
  if [ ! -e "$image" ]; then
    echo "Keine Bilddateien gefunden."
    exit 1
  fi

  echo "Bearbeite: $image"

  # Starte Pixelsortierer
  time ./Pixelsortierer 2 "$image"

  # Generiere Ausgabedateinamen basierend auf dem Bildnamen
  base_name="${image%.*}"
  output_file="${base_name}.mp4"

  # Versuche, Video mit ffmpeg zu erstellen
  if ffmpeg -framerate 60 -i outanim_%d.png -c:v libx265 -preset slow -crf 0 "$output_file"; then
    echo "Erfolgreich verarbeitet: $output_file"
  else
    echo "Fehler mit ffmpeg, wende Korrektur an..."
    if ffmpeg -framerate 60 -i outanim_%d.png -vf "pad=ceil(iw/2)*2:ceil(ih/2)*2" -c:v libx265 -preset slow -crf 0 "$output_file"; then
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
