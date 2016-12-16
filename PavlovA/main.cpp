#include "figures.h"
//#include <typeinfo>
#include <vector>
#include <memory>
#include <windows.h>
#include <algorithm>
#include <iterator>
#define NDEBUG
//#include <cassert>
#include <QtGlobal>

using namespace std;



class SomeClass
{
  public:
    SomeClass()
    {
      throw exception();
    }
};
class SomeOtherClass
{
  public:
    SomeOtherClass() {}
};

class MyClass
{
  public:
    MyClass() : a_(new SomeClass), b_(new SomeOtherClass) { }

    ~MyClass() throw()
    {
      //Было
      // delete a_;
      // delete b_;
    }

  private:
    //Было
    //    SomeClass *a_;
    //    SomeOtherClass *b_;
    //Надо
    std::unique_ptr<SomeClass> a_;
    std::unique_ptr<SomeOtherClass> b_;
};
class A
{
    int m_i;
    int m_j;
  public:
    A(int i, int j): m_i(i), m_j(j)
    {}
    bool operator==(const A &Rhs)
    {
      return m_i == Rhs.m_i && m_j == Rhs.m_j;
    }
};
class Node: public std::enable_shared_from_this<Node>
{
  private:
    typedef std::shared_ptr<Node> Child_t;
    typedef std::vector<Child_t> Children_t;
  private:
    Node *m_pParent;
    Children_t m_ChildList;
  public:
    Node(): m_pParent(nullptr)
    {
    }
    void AddChild(const Child_t &Child)
    {
      m_ChildList.push_back(Child);
      Child->m_pParent = this;
    }
    void RemoveChild(const Child_t &Child)
    {
      auto NewEnd = std::remove(std::begin(m_ChildList),
                                std::end(m_ChildList), Child);
      m_ChildList.erase(NewEnd, std::end(m_ChildList));
    }
    void SetParent(Node *pNewParent)
    {
      m_pParent->RemoveChild(shared_from_this());//
      pNewParent->AddChild(shared_from_this());  //
    }
    size_t ChildrenCount() const
    {
      return m_ChildList.size();
    }
    Node *Parent() const
    {
      return m_pParent;
    }
};
// пример для owner_before
class A1
{
    int i;
};
class B1
{
    int j;
};
class C: public A1, public B1
{};

struct Z {
  B1 *m_B;
  Z(): m_B(new B1)
  {}
};

const int ARR_SIZE = 3;
int main()
{
  setlocale(LC_ALL, "RUS");
  //
  // UNIQUE_PTR
  //
  /*Указатель на объект unique_ptr - замена auto_ptr
   * Объект класса unique_ptr является перемещаемым, но не копируемым, что в терминах С++ означает,
   * что у него определен оператор перемещения(operator=(T&&)) и конструктор перемещения, а оператор копирования
   * и конструктор копирования отсутствуют. После перемещения(равно как и при создании посредством конструктора
   * по умолчанию) unique_ptr содержит в себе nullptr.
   */
  cout << "Примеры использования  unique_ptr:"  << endl;
  unique_ptr<circle> ptr(new circle());
  ptr->set_square(5);
  unique_ptr<int> p(new int);
  *p = 10;
  //Указатель на массив объектов
  unique_ptr<int[]> parr(new int[ARR_SIZE]);
  parr[0] = 1;
  parr[1] = 2;
  parr[3] = 3;
  cout << "Указатель на переменную типа int: " << *p << ", указатель на элемент массива: " << parr[1] << ", вызов метода посредством указателя: square=" << ptr->get_square() << endl;
  /* также, в отличие от auto_ptr, unique_ptr позволяет задать пользовательскую операцию удаления,
   * что дает возможность работать не только с указателями, память под которые выделена с помощью new,
   * но и с любыми другими.
   */
  //  HANDLE File1 = ::CreateFile(L"test.txt",
  //        GENERIC_WRITE,
  //        FILE_SHARE_READ,
  //        NULL, CREATE_ALWAYS,
  //        FILE_ATTRIBUTE_NORMAL,
  //        NULL);
  //    {
  //        std::unique_ptr<void, decltype(std::ptr_fun(&::CloseHandle))>
  //            spHandle1(File1, std::ptr_fun(&::CloseHandle));
  //    }
  /*Борьба с утечками памяти
   * потеря экземпляра myclass из-за вброса исключения в конструкторе MyClass:
   * при раскрутке стека в результате возникновения исключения уничтожаются только
   * полностью созданные объекты, а MyClass таким не является – управление не доходит до тела конструктора.
   * В результате будут вызваны только деструкторы членов класса, а в случае MyClass деструкторы эти ничего не делают,
   * т.к. a_ и b_ являются обыкновенными указателями. Всё это приводит к утечке памяти.
   * Замена этих указателей на unique_ptr позволяет избежать этой ошибки.
   */
  //  try {
  //    MyClass myclass;
  //  } catch (...) {
  //    cout << "Ошибка выполнения." << endl;
  //  }
  //
  // SHARED_PTR
  /*
   * Этот тип указателя, в противовес unique_ptr, является разделяемым. Это значит, что он может быть скопирован
   *  и каждая его копия будет обращаться к одному и тому же указателю. При разрушении объекта shared_ptr ресурс,
   * хранящийся в нем, будет освобожден тогда и только тогда, когда не существует других объектов shared_ptr ссылающихся
   * на этот ресурс. Или другими словами: указатель будет освобожден тогда, когда будет уничтожена последняя копия
   * изначального shared_ptr, если хоть одна было создана. Это реализовано посредством счётчика ссылок.
  */
  //В отличие от unique_ptr shared_ptr не позволяет  хранить в себе указатели на массивы,
  //  shared_ptr<int[]> p_arr(new int[5]); //-> ошибка компиллятора
  // Также,как и unique_ptr, shared_ptr позволяет задать пользовательскую операцию удаления
  HANDLE File2 = ::CreateFile(L"test.txt",
                              GENERIC_WRITE,
                              FILE_SHARE_READ,
                              NULL, CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);
  {
    std::shared_ptr<void> spHandle2(File2, &::CloseHandle);
  }
  /* Все это происходит благодаря еще одной особенности shared_ptr – этот шаблонный класс зависит только от типа указателя,
   * т.е. тип операции удаления не влияет на инстанциацию шаблона! Это, также, означает, что shared_ptr созданный с одной
   * операцией удаления может быть присвоен другому shared_ptr с другой операцией удаления. Также, все, что описано выше для
   * пользовательской операции удаления справедливо и для пользовательского аллокатора.
  */
  //  STD::MAKE_SHARED и std::ALLOCATE_SHARED
  //
  //  Обе функции служат одной цели – создание shared_ptr в недрах реализации. allocate_shared отличается лишь тем,
  //  что позволяет задать пользовательский аллокатор(операцию выделения памяти) при создании shared_ptr
  //  Код с make_shared позволяет не использовать в своей записи new и указывать тип указателя лишь один раз.
  //Объявить указатель с помощью new
  std::shared_ptr<A> spFirst(new A(1, 2));
  //Объявить указатель с помощью make_shared
  auto spSecond = std::make_shared<A>(1, 2);
  Q_ASSERT(*spFirst == *spSecond);
  //
  //std::enable_shared_from_this - наследование от этого класса позволяет получать shared_ptr из this
  //Пример на использование std::enable_shared_from_this- class Node
  auto spRoot = std::make_shared<Node>();
  auto spChild = std::make_shared<Node>();
  spRoot->AddChild(spChild);
  Q_ASSERT(spChild->Parent() == spRoot.get());
  Q_ASSERT(spRoot->ChildrenCount() == 1);
  auto spAnotherRoot = std::make_shared<Node>();
  spChild->SetParent(spAnotherRoot.get());
  Q_ASSERT(spRoot->ChildrenCount() == 0);
  Q_ASSERT(spAnotherRoot->ChildrenCount() == 1);
  Q_ASSERT(spChild->Parent() == spAnotherRoot.get());
  //
  // OWNER_BEFORE -альтернативная версия оператора <.
  // Выполняет компиляторо-зависимое сравнение концептуально эквивалентное оператору <.
  // При этом соблюдается правило эквивалентности известное по оператору < - два shared_ptr(first и second)
  // эквивалентны, если выполняется следующее условие:
  // !first.owner_before(second) &&!second.owner_before(first)
  //
  //#1 - при сравнении объектов оператором < объекты не являются эквивалентными
  {
    auto spInitial = std::make_shared<C>();
    std::shared_ptr<void> spDerived = std::static_pointer_cast<B1>(spInitial);
    Q_ASSERT(spInitial != spDerived);
    Q_ASSERT(spInitial < spDerived || spInitial > spDerived);
    Q_ASSERT(!spInitial.owner_before(spDerived) && !spDerived.owner_before(spInitial));
  }
  //#2 два shared_ptr с разными типами данных и оператор < не может быть применен, в то время как owner_before отрабатывает правильно.
  auto spInitial = std::make_shared<Z>();
  std::shared_ptr<B1> spInner(spInitial, spInitial->m_B);
  Q_ASSERT(!spInitial.owner_before(spInner) &&
           !spInner.owner_before(spInitial));
  //
  // std::WEAK_PTR
  //Конструируется weak_ptr из существующего shared_ptr и его единственной целью является предоставление
  //пользователю информации об ассоциированным с ним shared_ptr без увеличения его счётчика ссылок.
  //Позволяет избежать циклической ссылки при использовании указателей для связи родитель-потомок.
  //При этом для указателя родителя на потомок используется shared_ptr, а для указателя потомка на родителя - weak_ptr
  //weak_ptr не позволяет работать с объектом напрямую, вместо этого используется метов lock().
  //
  //определение shared_ptr
  auto spShared = std::make_shared<int>();
//Один владелец ?
  Q_ASSERT(spShared.unique());
  //создание weak_ptr из shared_ptr
  std::weak_ptr<int> Weak(spShared);
  //является ли ассоциированный shared_ptr “живым” или же счетчик ссылок обнулен и объект не существует более.
  Q_ASSERT(Weak.expired() == false);
  //количество ссылок
  Q_ASSERT(Weak.use_count() == 1);
  //можно получить сам объект shared_ptr непосредственно, точнее его копию, при этом увеличится счётчик ссылок
  auto spClone = Weak.lock();
  Q_ASSERT(Weak.use_count() == 2);
  //удаляет объект, которым владеет указатель
  spShared.reset();
  //Не один владелец ?
  Q_ASSERT(!spShared.unique());
  Q_ASSERT(Weak.use_count() == 1);
  spClone.reset();
  Q_ASSERT(Weak.expired());
  //
  //dYNAMIC_POINTER_CAST , STATIC_POINTER_CAST и CONST_POINTER_CAST
  //дублируют функциоанал базовых операторов dynamic_cast, static_cast и const_cast применяемых к “голым” указателям.
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Пример с использованием  shared_ptr:
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  cout << endl << "Пример с использованием  shared_ptr:" << endl;
  typedef  vector <shared_ptr<shape>> Tobjset;
  Tobjset arr;
  //  arr.emplace_back(std::move(make_shared<triangle>()));
  arr.push_back(std::move(make_shared<triangle>()));
  dynamic_pointer_cast<triangle>(arr.back())->set_square(3, 4, 5);
  //  cout << "count "<<arr.back().use_count() << endl;
  //  arr.emplace_back(std::move(make_shared<rectangle>()));
  arr.push_back(std::move(make_shared<rectangle>()));
  dynamic_pointer_cast<rectangle>(arr.back())->set_square(3, 4);
  //  arr.emplace_back(std::move(make_shared<circle>()));
  arr.push_back(std::move(make_shared<circle>()));
  dynamic_pointer_cast<circle>(arr.back())->set_square(3);

  for (Tobjset::const_iterator it = arr.begin(); it != arr.end(); ++it) {
    cout << "Площадь фигуры " << (*it)->get_square() << endl;
  }

  cout << endl << endl;

  for (const auto &it : arr) {
    //Приведение вниз
    auto p = dynamic_pointer_cast<triangle>(it);
    //    shared_ptr<triangle> p = dynamic_pointer_cast<triangle>(it);

    if (p != nullptr) {
      p->set_square(6, 7, 8);
    } else {
      auto p = dynamic_pointer_cast<rectangle>(it);
      //      shared_ptr<rectangle> p = dynamic_pointer_cast<rectangle>(it);

      if (p != nullptr) {
        p->set_square(5, 4);
      } else {
        auto p = dynamic_pointer_cast<circle>(it);
        //        shared_ptr<circle> p = dynamic_pointer_cast<circle>(it);

        if (p != nullptr)
          p->set_square(10);
      }
    }

    cout << "Площадь фигуры " << it->get_square() << endl;
  }

  //Освободить память
  for (auto &it : arr)
    it.reset();

  return 0;
}
