#include "ft_hash.h"
#include <stdio.h>
int	test_destroy(t_hash_table **table)
{
	t_hash_table	*test_table;

	test_table = *table;
	ht_destroy(&test_table);
	if (test_table)
		return (ERROR);
	return (SUCCESS);
}

int	test_creation(t_hash_table **table)
{
	t_hash_table	*test_table;

	test_table = ht_create(10);
	if (!test_table)
		return (ERROR);
	printf("test_table: %p\n", test_table);
	if (test_table->size != 10)
		return (ERROR);
	printf("test_table size: %zu\n", test_table->size);
	if (test_table->filled != 0)
		return (ERROR);
	printf("test_table size: %zu\n", test_table->filled);
	if (!test_table->table)
		return (ERROR);
	printf("test_table->table: %p\n", test_table->table);
	if (test_table->table[10] != NULL)
		return (ERROR);
	printf("test_creation: OK\n");
	*table = test_table;
	return (SUCCESS);
}


int	test_get_hash(void)
{
	int res;

	res = get_hash("test");
	printf("hash of 'test' = %d\n", res);
	res = get_hash("test2");
	printf("hash of 'test2' = %d\n", res);
	res = get_hash("abitofalongerstring");
	printf("hash of 'abitofalongerstring' = %d\n", res);
	return (SUCCESS);
}

int	test_update(void)
{
	size_t			hash;
	int				index;	
	t_hash_table	*table;

	table = ht_create(10);
	if (!table)
		return (ERROR);
	hash  = get_hash("test");
	index = hash % table->size;
	
	printf("index: %d\n", index);
	char *test_value = ft_strdup("test_value");
	char *new_value = ft_strdup("new_value");
	if (ht_add(table, "test", test_value) == ERROR)
		return (ERROR);
	printf("------------");
	printf("\n");
	ht_print(table);
	printf("\n");

	ht_update_value(table, "test", new_value);
	printf("\n");

	ht_print(table);
	printf("\n");

	printf("------------");
	return (SUCCESS);
}

int	test_add(void)
{
	size_t			hash;
	int				index;	
	t_hash_table	*table;

	table = ht_create(10);
	if (!table)
		return (ERROR);
	hash  = get_hash("test");
	index = hash % table->size;
	printf("TEST_ADD\n");
	printf("index: %d\n", index);
	char *test_value = ft_strdup("test_value");
	if (ht_add(table, "test", test_value) == ERROR)
		return (ERROR);

	char *test_value2 = ft_strdup("test_value2");
	if (ht_add(table, "test2", test_value2) == ERROR)
		return (ERROR);
	
	ht_print(table);//
	ht_destroy(&table);
	return (SUCCESS);
}


//key ->if allocated before ht_add, free it 
	//substring to the first '=' - get len to it 
	//free after
//value -> needs t be allocated an dnot free'd unless we are destroying the table or entry
	//strdup of env_var + len to the  '='
	//don't free

int	test_get(void)
{
	char *value;
	t_hash_table	*table;

	table = ht_create(10);
	if (!table)
		return (ERROR);
	char *test_value = ft_strdup("test_value");
	ht_add(table, "test", test_value);
	value = ht_get(table, "test");
	if (!value)
		return (ERROR);
	printf("Retrieved value: %s\n", value);
	if (ft_strncmp(value, "test_value", ft_strlen(value)) != 0)
		return (ERROR);
	ht_destroy(&table);
	return (SUCCESS);
}

int	test_remove(void)
{
	t_hash_table	*table;

	table = ht_create(10);
	if (!table)
		return (ERROR);
	char *test_value = ft_strdup("test_value");
	ht_add(table, "test", test_value);
	if (ht_remove(table, "test") == ERROR)
		return (ERROR);
	if (ht_get(table, "test") != NULL)
		return (ERROR);
	ht_destroy(&table);
	return (SUCCESS);
}

int	test_rehash(void)
{
	t_hash_table	*table;

	table = ht_create(2);
	if (!table)
		return (ERROR);
	char *test_value = ft_strdup("test_value");
	if (ht_add(table, "test", test_value) == ERROR)
		return (ERROR);
	printf("table size: %zu\n", table->size);
	printf("table filled: %zu\n", table->filled);
	int	hash  = get_hash("test");//
	int	index = hash % table->size;//
	printf("index: %d\n", index);//

	char *test_value2 = ft_strdup("test_value2");
	if (ht_add(table, "test2", test_value2) == ERROR)
		return (ERROR);
	printf("table size: %zu\n", table->size);
	printf("table filled: %zu\n", table->filled);

	char *val_1 = ht_get(table, "test");//
	printf("first added: %s\n", val_1);//

	hash  = get_hash("test2");//
	index = hash % table->size;//
	printf("index: %d\n", index);//


	char *test_value3 = ft_strdup("test_value3");
	if (ht_add(table, "test3", test_value3) == ERROR)
		return (ERROR);
	printf("table size: %zu\n", table->size);
	printf("table filled: %zu\n", table->filled);
	
	if (ht_rehash(table) == ERROR)
		return (ERROR);
	printf("table size: %zu\n", table->size);
	printf("table filled: %zu\n", table->filled);
	ht_print(table);//
	ht_destroy(&table);
	return (SUCCESS);
}

int	main(void)
{
	t_hash_table	*table;

	// if (test_creation(&table) == ERROR)
	// 	return (ERROR);
	// if (test_destroy(&table) == ERROR)
	// 	return (ERROR);
	// test_get_hash();
	if (test_add() == ERROR)
		return (ERROR);
	if (test_update() == ERROR)
		return (ERROR);
	if (test_update() == ERROR)
		return (ERROR);
	// if (test_get() == ERROR)
	// 	return (ERROR);
	// if (test_remove() == ERROR)
	// 	return (ERROR);
	if (test_rehash() == ERROR)
		return (ERROR);
	return (0);
}