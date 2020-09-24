use strict;											#Подключаем
use lib qw(blib/lib lib);							#
use Archive::Zip;									#библиотеки
use FileHandle;										#

#Программа запускается на Linux-дистрибутивах
my $_2MiB = 2097152;								#2 Мегабайта в байтах
my $file_name = $ARGV[1];							#Имя файла
my $file_size = -s "$ARGV[1]";						#Получаем размер файла в байтах

if($ARGV[0] eq "--file") {							#Сравнение строк
	if(-d $file_name) {								#Проверка, является ли $file_name директорией
		warn "$0: Ошибка! \"${file_name}\": Это не файл, а директория.\n";
		exit 10;
	} else {
		my $file = FileHandle->new();				#Объявление файла
		if(!$file->open($file_name, 'r')) {			#Проверка, существует ли файл
			warn "$0: Ошибка! Файл \"${file_name}\" не существует.\n";
			exit 20;
		}

		if($file_size < $_2MiB) {					#Проверка, весит ли файл меньше, чем 2 МБайта
			warn "$0: Ошибка! Файл должен быть размером не менее 2МБайт.\n";
			exit 30;
		}

		binmode($file);								#Устанавливает дескриптор для чтения файла в текстовый режим
		my $data;									#Входные данные(содержимое файла)
		my $crc = 0;								#Контрольная сумма

		#Получение контрольной суммы файла
		while($file->read($data, 32768)) {
			$crc = Archive::Zip::computeCRC32($data, $crc);
		}

		my $file_crc = sprintf("%X", $crc);			#Преобразуем в 16-ричное число
		printf("$file_crc\n");						#Вывод результата на экран
		exit 0;
	}
} else {
	warn "$0: Ошибка! Программе переданы некорректные параметры запуска.\nПример: perl crc32.pl --file \"имя_файла\"\n";
	exit 40;
}