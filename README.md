# JsonParser2

**A tool for deserializing objects from json files**

## Typical Usage


### Single Object

Call the function `parsedObject` templated on the desired class and give the path to the json file as an argument.
```
int main()
{
  auto const person = JsonParser::parsedObject<Person>("SavedPerson.json");
  std::cout << person.name << " is " << person.age << std::endl;
}
```

### Multiple Objects

Call the function `parsedList` to get a list of your objects.
```
int main()
{
  auto const people = JsonParser::parsedList<Person>("SavedPeople.json");
  for (auto const& person : people)
    std::cout << person.name << " is " << person.age << std::endl;
}
```

### Abstract Objects

Call the function `parsedObject` templated on a unique pointer to your abstract class to get a unique pointer of abstract type to a concrete object
```
int main()
{
  Exposable<IShape>::register_helper<IShapeExposingHelper>();
  using IShapeUP = std::unique_ptr<IShape>;
  auto const abstractShape = JsonParser::parsedObject<IShapeUP>("AbstractShape.json");
  std::cout << abstractShape->draw() << std::endl;
}
```

# All You Need

## To parse a concrete object

is...

1. Your `class`
```
struct Person
{
  std::string name;
  int age = 0;
};
```

2. Your class should inherit from the class `Exposable` templated on itself
```
struct Person : Exposable<Person>
{
  std::string name;
  int age = 0;
};
```
3. And it must provide a public method `expose`
```
struct Person : Exposable<Person>
{
  std::string name;
  int age = 0;
  static void expose();
};
```
4. The body of the method should list all the pairs of `tags` and `pointers to members` you want to deserialize
```
struct Person : Exposable<Person>
{
  std::string name;
  int age = 0;
  static void expose()
  {
    Exposable<Person>::expose("name", &Person::name);
    Exposable<Person>::expose("age", &Person::age);
  }
};
```
See [tests](https://github.com/nestoroprysk/JsonParser2/blob/master/Tests/BasicTests.cpp) for further examples.

## To parse an abstract object

TODO: add detailed manual

See [tests](https://github.com/nestoroprysk/JsonParser2/blob/master/Tests/AdvancedTests.cpp) for further examples.

## Supported Types

### Basic
- std::string
- bool
- std::unique_ptr
### Integral
- shor
- unsigned short
- int
- unsigned int
- long
- unsigned long
- long long
- unsigned long long 
### Containers
- std::vector
- std::list
