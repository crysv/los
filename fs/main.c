int main()
{
    void* malloc = kmalloc(100);
    kfree(malloc);
    puts("hello\n");
    some_call();
	return 0;
}
