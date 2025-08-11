#!/bin/bash


set -e  # Прерывать при ошибках

# ---- Конфигурация ----
REQUIRED_PKGS=("gcc" "make" "cmake")  # Базовые зависимости
CHAT_PORT=8888                                   # Порт по умолчанию
BROADCAST_ADDR=$(ip route | awk '/default/ {print $3}' | awk -F'.' '{print $1"."$2"."$3".255"}')

# ---- Функции ----
check_deps() {
    echo "🔍 Проверка зависимостей..."
    for pkg in "${REQUIRED_PKGS[@]}"; do
        if ! dpkg -l | grep -q "^ii  $pkg "; then
            echo "⚠️ Устанавливаем $pkg..."
            sudo apt-get install -y "$pkg"
        fi
    done
}



compile_chat() {
    echo "🛠️ Компиляция программы..."
    mkdir -p build
    cd build || exit 1
    cmake ..
    make
    cd ..
}

show_usage() {
    echo -e "\n💡 Как использовать:"
    echo "  $ ./setup_chat.sh          # Настройка окружения"
    echo "  $ ./build/chat <PORT> <NICK>  # Запуск чата"
    echo -e "\nПример:"
    echo "  $ ./build/chat 8888 Alice"
}

clear
echo "🔄 Начало настройки окружения для UDP-чата..."

check_deps
compile_chat

echo -e "\n✅ Настройка завершена!"
echo "   Broadcast адрес: $BROADCAST_ADDR"
echo "   Рекомендуемый порт: $CHAT_PORT"

show_usage