int main()
{
    void* malloc = kmalloc(100);
    kfree(malloc);
    puts("hello");
    some_call();
	return 0;
}
