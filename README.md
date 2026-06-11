# RU

## ⚡ Tether

**Tether** — инженерный калькулятор для проектировщиков электрики, АСУ ТП и систем ТГВ. Программа помогает выполнять расчёты и подбирать электрооборудование:

- 🔹 Расчёт тока.  
- 🔹 Подбор автоматических выключателей.  
- 🔹 Выбор сечения кабеля по допустимому току (ПУЭ) и проверка падения напряжения.  
- 🔹 Подбор электродвигателя по ближайшей мощности или току.
- 🔹 Вспомогательные функции для отдела автоматизации: подбор модулей ПЛК вентиляции, подбор элементов автоматизации.
- 🔹 Вспомогательные функции для коммуникации с верхним уровнем: создание карты регистров ModBus, генерация кода ST. 

Все физические величины хранятся и вычисляются в **Международной системе единиц (СИ)** — ватты, амперы, вольты, омы на метр, эффективность в долях единицы.

---

## 🛠️ Сборка

Проект написан на **C++17**. Для сборки требуется компилятор с поддержкой C++17.

1. Склонируйте репозиторий.  
2. Откройте решение в Visual Studio (или соберите через CMake).  
3. Убедитесь, что корневая папка `Tether/src` добавлена в пути включения заголовков.  
4. Скомпилируйте и запустите.  

---

## 🧪 Использование

Пример:

```cpp
#include "tether.h"

int main() {
    Tether::Calc::CalculateCurrentContext ctx;

    ctx.power = 1500.0; // Мощность 1500 Вт
    ctx.voltage = 380.0; // НАпряжение 380 В
    ctx.cosPhi = 0.85; // Косинус фи
    ctx.eff = 0.9; // КПД
    ctx.selectFromCatalog = false; // Не выбирать из каталога
    ctx.loadType = Tether::Calc::LoadType::THREE_PHASE; // Тип нагрузки - трёхфазная

    auto catalog = Tether::Data::loadMotorCatalog(); // Загрузить каталоги
    auto result = Tether::Calc::currentByContext(catalog, ctx); // Рассчитать ток по заданному контексту

    return 0;
}
```
## 📄 Лицензия
Проект создан в учебных целях. Свободное использование при соблюдении авторства.

# EN

## ⚡ Tether

**Tether** is an engineering calculator for electrical, automation (SCADA/PLC), and HVAC system designers. The application assists with calculations and selection of electrical equipment:

- 🔹 Current calculation.  
- 🔹 Selection of circuit breakers.  
- 🔹 Cable cross-section selection based on permissible current (per PUE/Rules for Electrical Installations) and voltage drop verification.  
- 🔹 Electric motor selection by nearest power rating or current.  
- 🔹 Auxiliary functions for automation engineers: PLC I/O module selection for ventilation systems, automation component sizing.  
- 🔹 Utilities for upper-level integration: Modbus register map generation, Structured Text (ST) code generation.  

All physical quantities are stored and computed in the **International System of Units (SI)** — watts, amperes, volts, ohms per meter, and efficiency as a dimensionless fraction.

---

## 🛠️ Building

The project is written in **C++17**. A C++17-compliant compiler is required.

1. Clone the repository.  
2. Open the solution in Visual Studio (or build via CMake).  
3. Ensure the root source folder `Tether/src` is added to your include paths.  
4. Compile and run.  

---

## 🧪 Usage

Example:

```cpp
#include "tether.h"

int main() {
    Tether::Calc::CalculateCurrentContext ctx;

    ctx.power = 1500.0; // Power: 1500 W
    ctx.voltage = 380.0; // Voltage: 380 V
    ctx.cosPhi = 0.85; // Power factor (cos φ)
    ctx.eff = 0.9; // Efficiency
    ctx.selectFromCatalog = false; // Do not select from catalog
    ctx.loadType = Tether::Calc::LoadType::THREE_PHASE; // Load type: three-phase

    auto catalog = Tether::Data::loadMotorCatalog(); // Load motor catalog
    auto result = Tether::Calc::currentByContext(catalog, ctx); // Calculate current

    return 0;
}
```
## 📄 License
This project is created for educational purposes. Free use is permitted provided original authorship is retained.
