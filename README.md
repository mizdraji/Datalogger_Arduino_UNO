# Datalogger Arduino UNO

Registrador de datos basado en **Arduino UNO** con shield de datalogger (RTC + tarjeta SD). Mide voltaje en el pin analógico **A0**, promedia varias lecturas y guarda fecha, hora y voltaje en archivos **CSV** en la SD.

## Características

- Lectura de voltaje en `A0` con promedio de 10 muestras
- Marca de tiempo desde RTC **DS1307**
- Registro en SD cada **60 segundos** (configurable)
- Archivos CSV con encabezado: `Fecha, Hora, Voltaje (V)`
- Nombres automáticos: `sleeplg1.csv`, `sleeplg2.csv`, … (formato compatible FAT 8.3)

## Hardware necesario

| Componente | Notas |
|------------|--------|
| Arduino UNO | |
| Shield datalogger | RTC DS1307 + lector SD |
| Tarjeta microSD | Formateada en FAT16/FAT32 |
| Divisor / sensor de voltaje | Conectado a **A0** (máx. 5 V en el pin del Arduino) |
| Batería CR1220 (opcional) | Para mantener la hora del RTC sin alimentación |

## Conexiones (pines usados en el sketch)

| Pin / señal | Función |
|-------------|---------|
| `A0` | Entrada analógica de voltaje |
| `10` (`chipSelect`) | Selección de chip SD (típico del shield) |
| I2C (`SDA` / `SCL`) | RTC DS1307 (vía `Wire`) |

## Dependencias (librerías Arduino)

Instalar desde el **Gestor de librerías** del IDE de Arduino:

- [RTClib](https://github.com/adafruit/RTClib) (Adafruit)
- **SD** (incluida con el core de Arduino)
- **SPI** y **Wire** (incluidas)

## Configuración

Parámetros principales en `Datalogger_Arduino_UNO.ino`:

| Constante | Valor por defecto | Descripción |
|-----------|-------------------|-------------|
| `SerialSpeed` | `9600` | Velocidad del monitor serie |
| `interval` | `60000` | Intervalo entre registros (ms) |
| `numLecturas` | `10` | Lecturas para el promedio de voltaje |
| Factor ADC | `4.94 / 1023.0` | Conversión de cuenta ADC a voltios |

Para cambiar el intervalo de muestreo, modificar `interval` (por ejemplo, `30000` = 30 s).

## Uso

1. Montar el shield en el Arduino UNO e insertar la tarjeta SD.
2. Conectar la señal de voltaje a **A0** (respetando el rango 0–5 V del ADC).
3. Abrir el sketch en el IDE de Arduino y cargarlo en la placa.
4. Abrir el **Monitor Serie** a **9600 baud** para ver el estado y las lecturas.
5. Tras el arranque se crea un CSV nuevo; cada minuto se añade una línea.

### Primera puesta en marcha del RTC

Si el RTC no tiene hora válida, el programa muestra `RTC no está activado!` y se detiene. En ese caso, descomentar **solo una vez** en `setup()`:

```cpp
RTC.adjust(DateTime(__DATE__, __TIME__));
```

Volver a compilar y cargar con la hora del PC en el momento de la compilación. Luego volver a comentar esa línea para no resetear la hora en cada carga.

## Formato del archivo CSV

Ejemplo de `sleeplg1.csv`:

```csv
Fecha, Hora, Voltaje (V)
15/5/2026, 14:30:0, 3.256
15/5/2026, 14:31:0, 3.251
```

Los archivos se pueden abrir en Excel, LibreOffice Calc o cualquier herramienta de análisis de datos.

## Monitor serie

Mensajes habituales:

- `iniciando...` — arranque
- `SD inicializada!` — SD lista
- `Archivo creado: sleeplgN.csv` — nuevo archivo de sesión
- `Voltaje Promedio: …` — lectura actual
- `Datos añadidos al archivo CSV` — registro correcto

## Solución de problemas

| Mensaje | Posible causa |
|---------|----------------|
| `RTC no está activado!` | RTC sin batería, hora no configurada o fallo I2C |
| `No se pudo inicializar la SD...` | SD mal insertada, sin formato FAT o pin CS incorrecto |
| `Error al crear el archivo CSV` | SD llena, solo lectura o nombre no válido |
| `Error al abrir el archivo CSV` | SD retirada durante el funcionamiento o corrupción |

## Licencia

Consultar el repositorio o el autor del proyecto para condiciones de uso.
