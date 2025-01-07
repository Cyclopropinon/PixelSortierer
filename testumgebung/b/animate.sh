#!/bin/bash

# Initialisiere Zähler
counter=1

FPS=30

# Auflösungen definieren
get_resolution() {
  case "$1" in
    --460p) echo "640:460" ;;
    --720p) echo "1280:720" ;;
    --1080p) echo "1920:1080" ;;
    --4k) echo "3840:2160" ;;
    --8k) echo "7680:4320" ;;
    *) echo "" ;;
  esac
}

# Überprüfe auf Skalierungsoptionen
resolution=""
for arg in "$@"; do
  res=$(get_resolution "$arg")
  if [ -n "$res" ]; then
    resolution="$res"
    break
  fi
done

# Überprüfe, ob mindestens eine Bilddatei existiert
shopt -s nullglob
images=( *.png *.jpg *.jpeg )
if [ ${#images[@]} -eq 0 ]; then
  echo "Keine Bilddateien gefunden."
  exit 1
fi

echo $images

# Filtere Bilder, die mit "outanim" beginnen
images=( $(printf "%s\n" "${images[@]}" | grep -v '^outanim') )
if [ ${#images[@]} -eq 0 ]; then
  echo "Keine geeigneten Bilddateien gefunden."
  exit 1
fi

echo "b"
echo "$resolution"

echo $images

# Auflösungen definieren
get_resolution() {
  case "$1" in
    --460p) echo "640:460" ;;
    --720p) echo "1280:720" ;;
    --1080p) echo "1920:1080" ;;
    --4k) echo "3840:2160" ;;
    --8k) echo "7680:4320" ;;
    *) echo "" ;;
  esac
}

# Bilder herunterskalieren, falls eine Auflösung angegeben ist und sie zu groß sind
if [ -n "$resolution" ]; then
  echo "Überprüfe und skaliere Bilder auf $resolution, falls nötig, unter Beibehaltung des Bildverhältnisses..."
  max_width=$(echo "$resolution" | cut -d':' -f1)
  max_height=$(echo "$resolution" | cut -d':' -f2)

  new_images=()
  for image in "${images[@]}"; do
    dimensions=$(identify -format "%wx%h" "$image" 2>/dev/null)
    width=$(echo "$dimensions" | cut -d'x' -f1)
    height=$(echo "$dimensions" | cut -d'x' -f2)

    if [ "$width" -gt "$max_width" ] || [ "$height" -gt "$max_height" ]; then
      echo "Skaliere $image von ${width}x${height} auf maximal ${max_width}x${max_height}..."
      ext="${image##*.}"
      base="${image%.*}"
      scaled_image="${base}_scaled.png"
      if ! ffmpeg -i "$image" -vf "scale='if(gt(iw/ih\,$max_width/$max_height),$max_width,-2)':"\
"'if(gt(iw/ih\,$max_width/$max_height),-2,$max_height)'" "$scaled_image"; then
        echo "Fehler beim Skalieren von $image."
        exit 1
      fi
      new_images+=("$scaled_image")
    else
      new_images+=("$image")
    fi
  done
  images=("${new_images[@]}")
fi

echo "c"

# Schleife durch alle Bilddateien im aktuellen Verzeichnis
for image in "${images[@]}"; do
  # Prüfe, ob Dateien existieren
  if [ ! -e "$image" ]; then
    echo "Keine Bilddateien gefunden."
    exit 1
  fi

  echo "Bearbeite: $image"

  # Starte Pixelsortierer
  time ./Pixelsortierer -1 "$image"

  # Generiere Ausgabedateinamen basierend auf dem Bildnamen
  base_name="${image%.*}"
  output_file="${base_name}.mp4"

  # Versuche, Video mit ffmpeg zu erstellen
  if ffmpeg -framerate $FPS -i outanim_%d.png -c:v libx264 -crf 0 -preset veryslow -pix_fmt yuv420p "$output_file"; then
  #if ffmpeg -framerate $FPS -i outanim_%d.png -c:v libx265 -preset slow -crf 0 "$output_file"; then
    echo "Erfolgreich verarbeitet: $output_file"
  else
    echo "Fehler mit ffmpeg, wende Korrektur an..."
    if ffmpeg -framerate $FPS -i outanim_%d.png -vf "pad=ceil(iw/2)*2:ceil(ih/2)*2" -c:v libx264 -crf 0 -preset veryslow -pix_fmt yuv420p "$output_file"; then
    #if ffmpeg -framerate $FPS -i outanim_%d.png -vf "pad=ceil(iw/2)*2:ceil(ih/2)*2" -c:v libx265 -preset slow -crf 0 "$output_file"; then
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
