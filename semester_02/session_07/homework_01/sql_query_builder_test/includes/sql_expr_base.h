// sql_expr_base.h

#ifndef SQL_EXPR_BASE_H_IN_MY_PROJECT
#define SQL_EXPR_BASE_H_IN_MY_PROJECT

#include "my_types.h"
#include <cctype>
#include <functional>
#include <iterator>
#include <string>
#include <type_traits>

namespace sql
{
	constexpr const char* keywords[]
	{
		"ABSOLUTE","ACTION","ADD","ADMIN","AFTER","AGGREGATE","ALIAS","ALL","ALLOCATE","ALTER","AND","ANY","ARE","ARRAY",
		"AS","ASC","ASSERTION","ASSERTION","AT","ATOMIC","AUTHORIZATION",
		"BEFORE","BEGIN","BIGINT","BINARY","BIT","BLOB","BOOLEAN","BOTH","BREADTH","BY",
		"CALL","CASCADE","CASCADED","CASE","CAST","CATALOG","CHAR","CHARACTER","CHECK","CLASS","CLOB","CLOSE","COLLATE",
		"COLLATION","COLLECT","COLUMN","COMMIT","COMPLETION","CONDITION","CONNECT","CONNECTION","CONSTRAINT","CONSTRAINTS",
		"CONSTRUCTOR","CONTAINS","CONTINUE","CORRESPONDING","CREATE","CROSS","CUBE","CURRENT","CURRENT_DATE","CURRENT_PATH",
		"CURRENT_ROLE","CURRENT_TIME","CURRENT_TIMESTAMP","CURRENT_USER","CURSOR","CYCLE",
		"DATA","DATALINK","DATE","DAY","DEALLOCATE","DEC","DECIMAL","DECLARE","DEFAULT","DEFERRABLE","DELETE","DEPTH","DEREF",
		"DESC","DESCRIPTOR","DESTRUCTOR","DIAGNOSTICS","DICTIONARY","DISCONNECT","DO","DOMAIN","DOUBLE","DROP","ELEMENT",
		"END_EXEC","EQUALS","ESCAPE","EXCEPT","EXCEPTION","EXECUTE","EXIT","EXPAND","EXPANDING",
		"FALSE","FIRST","FLOAT","FOR","FOREIGN","FREE","c","FUNCTION","FUSION",
		"GENERAL","GET","GLOBAL","GOTO","GROUP","GROUPING",
		"HANDLER","HASH","HOUR",
		"IDENTITY","IF","IGNORE","IMMEDIATE","IN","INDICATOR","INITIALIZE","INITIALLY","INNER","INOUT","INPUT","INSERT","INT",
		"INTEGER","INTERSECT","INTERSECTION","INTERVAL","INTO","IS","ISOLATION","ITERATE",
		"JOIN",
		"KEY",
		"LANGUAGE","LARGE","LAST","LATERAL","LEADING","LEAVE","LEFT","LESS","LEVEL","LIKE","LIMIT","LOCAL","LOCALTIME",
		"LOCALTIMESTAMP","LOCATOR","LOOP",
		"MATCH","MEMBER","MEETS","MERGE","MINUTE","MODIFIES","MODIFY","MODULE","MONTH","MULTISET",
		"NAMES","NATIONAL","NATURAL","NCHAR","NCLOB","NEW","NEXT","NO","NONE","NORMALIZE","NOT","NULL","NUMERIC",
		"OBJECT","OF","OFF","OLD","ON","ONLY","OPEN","OPERATION","OPTION","OR","ORDER","ORDINALITY","OUT","OUTER","OUTPUT",
		"PAD","PARAMETER","PARAMETERS","PARTIAL","PATH","PERIOD","POSTFIX","PRECEDES","PRECISION","PREFIX","PREORDER","PREPARE",
		"PRESERVE","PRIMARY","PRIOR","PRIVILEGES","PROCEDURE","PUBLIC",
		"READ","READS","REAL","RECURSIVE","REDO","REF","REFERENCES","REFERENCING","RELATIVE","REPEAT","RESIGNAL","RESTRICT",
		"RESULT","RETURN","RETURNS","REVOKE","RIGHT","ROLE","ROLLBACK","ROLLUP","ROUTINE","ROW","ROWS",
		"SAVEPOINT","SCHEMA","SCROLL","SEARCH","SECOND","SECTION","SELECT","SEQUENCE","SESSION","SESSION_USER","SET","SETS",
		"SIGNAL","SIZE","SMALLINT","SPECIFIC","SPECIFICTYPE","SQL","SQLEXCEPTION","SQLSTATE","SQLWARNING","START","STATE",
		"STATIC","STRUCTURE","SUBMULTISET","SUCCEEDS","SUM","SYSTEM_USER",
		"TABLE","TABLESAMPLE","TEMPORARY","TERMINATE","THAN","THEN","TIME","TIMESTAMP","TIMEZONE_HOUR","TIMEZONE_MINUTE",
		"TO","TRAILING","TRANSACTION","TRANSLATION","TREAT","TRIGGER","TRUE",
		"UESCAPE","UNDER","UNDO","UNION","UNIQUE","UNKNOWN","UNTIL","UPDATE","USAGE","USER","USING",
		"VALUE","VALUES","VARCHAR","VARIABLE","VARYING","VIEW",
		"WHEN","WHENEVER","WHERE","WHILE","WITH","WRITE","YEAR","ZONE"
	};

	namespace keyword
	{
		constexpr decltype(auto) AS{ "AS" };
		constexpr decltype(auto) SELECT{ "SELECT" };
		constexpr decltype(auto) FROM{ "FROM" };
		constexpr decltype(auto) WHERE{ "WHERE" };
	} /// namespace keyword

	namespace selection
	{
		constexpr decltype(auto) equal() { return "="; }
		constexpr decltype(auto) unequal() { return "!="; }
		constexpr decltype(auto) greater() { return ">"; }
		constexpr decltype(auto) less() { return "<"; }
		constexpr decltype(auto) greater_or_equal() { return ">="; }
		constexpr decltype(auto) less_or_equal() { return  "<="; }
		/*
		constexpr decltype(auto) LIKE() { return "LIKE"; }
		constexpr decltype(auto) IN() { return "IN"; }
		constexpr decltype(auto) BETWEEN() { return "BETWEEN"; }
		*/
	} /// namespace oper

	namespace character
	{
		constexpr decltype(auto) comma() { return static_cast<char>(my::ascii::comma); }
		constexpr decltype(auto) space() { return static_cast<char>(my::ascii::space); }
		constexpr decltype(auto) asterisk() { return static_cast<char>(my::ascii::asterisk); }
		constexpr decltype(auto) semicolon() { return static_cast<char>(my::ascii::semicolon); }
		constexpr decltype(auto) single_quote() { return static_cast<char>(my::ascii::single_quote); }
	} /// namespace character

	namespace special_character
	{
		constexpr decltype(auto) endl() { return '\n'; };
	} /// namespace special_character 

	namespace logic
	{
		constexpr decltype(auto) AND() { return "AND"; }
		constexpr decltype(auto) OR() { return "OR"; }
		constexpr decltype(auto) NOT() { return "NOT"; }
	} /// struct namespace logic

	using select_operator = std::function<const char* ()>;
	template<typename T>
	using logic_operator = std::function<T()>;

	namespace helper
	{
		template<typename Container,
			typename = std::enable_if_t<std::is_same_v<typename Container::value_type, char>>>
		decltype(auto) toupper(const Container& cont)
		{
			Container cont_temp{ std::move(cont) };
			std::transform(std::cbegin(cont_temp), std::cend(cont_temp), std::begin(cont_temp), ::toupper);
			return cont_temp;
		}

		auto check(const std::string& value) -> bool
		{
			/// Алгоритм проверки имен и псевдонимов
			return true; /// считаем, что проверка прошла успешно
		}
	} /// namespace helper
} /// namespace sql

#endif // !SQL_EXPR_BASE_H_IN_MY_PROJECT

/// Диалекты SQL — это вариации стандарта SQL, используемые различными системами управления
/// базами данных. Они могут включать дополнительные функции, собственные расширения или
/// оптимизации, специфичные для платформы поставщика.

/// T-SQL (Transact-SQL, Microsoft SQL Server) - диалект SQL, разработанный Microsoft 
/// для работы с Microsoft SQL Server. Используется в крупных корпоративных системах, 
/// работающих на Windows, и в проектах, использующих технологию NET.

/// Конвенции имен. Стандарт SQL не содержит никаких точных указаний по наименованиям, 
/// поэтому нужно следовать следующим основным принципам, в том числе и в синтаксисе SQL запросов 
/// UPDATE. Выбирайте имя, которое содержит смысл и имеет описательный характер. Используйте для
/// всех объектов в базе данных SQL либо заглавные буквы, либо строчные, поскольку некоторые
/// серверы баз данных чувствительны к регистру.

/// Идентификаторы и ключевые слова SQL должны начинаться с буквы(a - z, хотя допускаются также 
/// не латинские буквы и буквы с диакритическими знаками) или подчёркивания(_). Последующими
/// символами в идентификаторе или ключевом слове могут быть буквы, цифры(0 - 9),
/// знаки доллара($) или подчёркивания.Заметьте, что строго следуя букве стандарта SQL, знаки
/// доллара нельзя использовать в идентификаторах, так что их использование вредит переносимости
/// приложений. В стандарте SQL гарантированно не будет ключевых слов с цифрами и начинающихся
/// или заканчивающихся подчёркиванием, так что идентификаторы такого вида защищены от возможных
/// конфликтов с будущими расширениями стандарта.
/// 
/// Ключевые слова и идентификаторы без кавычек воспринимаются системой без учёта регистра.
/// Есть и другой тип идентификаторов: отделённые идентификаторы или идентификаторы в кавычках.
/// Они образуются при заключении обычного набора символов в двойные кавычки("). Такие идентификаторы
/// всегда будут считаться идентификаторами, но не ключевыми словами. Так "select" можно использовать
/// для обозначения столбца или таблицы «select», тогда как select без кавычек будет воспринят 
/// как ключевое слово и приведёт к ошибке разбора команды в месте, где ожидается имя таблицы или столбца. 
/// Тот же пример можно переписать с идентификаторами в кавычках следующим образом:
/// Идентификаторы в кавычках могут содержать любые символы, за исключением символа с кодом 0.
/// Чтобы включить в такой идентификатор кавычки, продублируйте их. Это позволяет создавать таблицы
/// и столбцы с именами, которые иначе были бы невозможны, например, с пробелами или амперсандами.
/// Ограничение длины при этом сохраняется.
/// Идентификатор, заключённый в кавычки, становится зависимым от регистра, тогда как идентификаторы
/// без кавычек всегда переводятся в нижний регистр. Например:
/// 1. Идентификаторы FOO, foo и "foo" считаются одинаковыми в PostgreSQL, но "Foo" и "FOO" 
///    отличны друг от друга и от предыдущих трёх. 
/// Приведение имён без кавычек к нижнему регистру, как это делает PostgreSQL, несовместимо со 
/// стандартом SQL, который говорит о том, что имена должны приводиться к верхнему регистру.
/// То есть, согласно стандарту foo должно быть эквивалентно "FOO", а не "foo".
/// Поэтому при создании переносимых приложений рекомендуется либо всегда заключать определённое
/// имя в кавычки, либо не заключать никогда.

/// Литералы SQL. Термин литералы относится к фиксированным значениям данных.
/// SQL распознает четыре типа литералов:
///  1. Числовые значения.
///  2. Строки символов.
///  3. Дата или время.
///  4. Логическое значение.
/// Например:
///  * 100, -120, 544, 03, -458, 25, 3e2, 5E-2 - являются действительными числовыми литералами;
///  * 'США', '2000', 'SQL Синтаксис', '1 января 1981'  - являются действительными строками символов
///    (должны быть заключены в одинарные кавычки(' '));
///  * TRUE, FALSE - логические литералы;
///  * 'JAN-28-1976 21:12:40:00' - литерал даты.

/// Язык SQL представляет собой совокупность операторов, инструкций, вычисляемых функций. 
/// Согласно общепринятому стилю программирования, операторы (и другие зарезервированные слова) 
/// в SQL обычно рекомендуется писать прописными буквами.
/// Операторы SQL делятся на:
/// 1. Операторы определения данных(Data Definition Language, DDL):
///    1.1. CREATE создаёт объект базы данных(саму базу, таблицу, представление, пользователя 
///         и так далее).
///    1.2. ALTER изменяет объект.
///    1.3. DROP удаляет объект.
/// 2. Операторы манипуляции данными(Data Manipulation Language, DML):
///    2.1. SELECT выбирает данные, удовлетворяющие заданным условиям.
///    2.2. INSERT добавляет новые данные.
///    2.3. UPDATE изменяет существующие данные.
///    2.4. DELETE удаляет данные.
/// 3. Операторы определения доступа к данным(Data Control Language, DCL):
///    3.1. GRANT предоставляет пользователю(группе) разрешения на определённые операции с объектом.
///    3.2. REVOKE отзывает ранее выданные разрешения.
///    3.3. DENY задаёт запрет, имеющий приоритет над разрешением.
/// 4. Операторы управления транзакциями(Transaction Control Language, TCL):
///    4.1. COMMIT применяет транзакцию.
///    4.2. ROLLBACK откатывает все изменения, сделанные в контексте текущей транзакции.
///    4.3. SAVEPOINT делит транзакцию на более мелкие участки.

/// SELECT [DISTINCT | DISTINCTROW | ALL] select_expression, ...
/// FROM table_references
/// [WHERE expr_pred]
/// [GROUP BY{ unsigned_integer | col_name | formula }]
/// [HAVING where_definition]
/// [ORDER BY{ unsigned_integer | col_name | formula }[ASC | DESC], ...]

/// GROUP BY
/// [[GROUP BY(SQL) | GROUP BY]] — необязательное предложение оператора SELECT, для группировки строк
/// по результатам агрегатных функций(MAX, SUM, AVG, …).	Необходимо, чтобы в предложении SELECT были 
/// заданы только требуемые в выходном потоке столбцы, перечисленные в GROUP BY и / или агрегированные
/// значения.Распространённая ошибка — указание в предложении SELECT столбца, пропущенного в GROUP BY.

/// HAVING
/// Необязательное предложение оператора SELECT для отбора групп, получающихся в результате
/// GROUP BY. При указании HAVING <условия> можно указывать условия на столбцах, указанных в GROUP BY, 
/// и значениях агрегатных функций, вычисленных для каждой группы, образованной GROUP BY.

/// ORDER BY
/// Необязательное предложение операторов SELECT и UNION, который означает что операторы SELECT,
/// UNION возвращают набор строк, отсортированных по значениям одного или более столбцов.
/// Его можно применять как к числовым столбцам, так и к строковым.В последнем случае, сортировка будет
/// происходить по алфавиту.	Использование предложения ORDER BY является единственным способом отсортировать
/// результирующий набор строк.Без этого предложения СУБД может вернуть строки в любом порядке.
/// Если упорядочение необходимо, ORDER BY должен присутствовать в SELECT, UNION. Сортировка может 
/// производиться как по возрастанию, так и по убыванию значений. Параметр ASC(по умолчанию) устанавливает
/// порядок сортировки по возрастанию, от меньших значений к большим. Параметр DESC устанавливает порядок
/// сортировки по убыванию, от больших значений к меньшим.

