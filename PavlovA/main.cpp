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
      //����
      // delete a_;
      // delete b_;
    }

  private:
    //����
    //    SomeClass *a_;
    //    SomeOtherClass *b_;
    //����
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
// ������ ��� owner_before
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
  /*��������� �� ������ unique_ptr - ������ auto_ptr
   * ������ ������ unique_ptr �������� ������������, �� �� ����������, ��� � �������� �++ ��������,
   * ��� � ���� ��������� �������� �����������(operator=(T&&)) � ����������� �����������, � �������� �����������
   * � ����������� ����������� �����������. ����� �����������(����� ��� � ��� �������� ����������� ������������
   * �� ���������) unique_ptr �������� � ���� nullptr.
   */
  cout << "������� �������������  unique_ptr:"  << endl;
  unique_ptr<circle> ptr(new circle());
  ptr->set_square(5);
  unique_ptr<int> p(new int);
  *p = 10;
  //��������� �� ������ ��������
  unique_ptr<int[]> parr(new int[ARR_SIZE]);
  parr[0] = 1;
  parr[1] = 2;
  parr[3] = 3;
  cout << "��������� �� ���������� ���� int: " << *p << ", ��������� �� ������� �������: " << parr[1] << ", ����� ������ ����������� ���������: square=" << ptr->get_square() << endl;
  /* �����, � ������� �� auto_ptr, unique_ptr ��������� ������ ���������������� �������� ��������,
   * ��� ���� ����������� �������� �� ������ � �����������, ������ ��� ������� �������� � ������� new,
   * �� � � ������ �������.
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
  /*������ � �������� ������
   * ������ ���������� myclass ��-�� ������ ���������� � ������������ MyClass:
   * ��� ��������� ����� � ���������� ������������� ���������� ������������ ������
   * ��������� ��������� �������, � MyClass ����� �� �������� � ���������� �� ������� �� ���� ������������.
   * � ���������� ����� ������� ������ ����������� ������ ������, � � ������ MyClass ����������� ��� ������ �� ������,
   * �.�. a_ � b_ �������� ������������� �����������. �� ��� �������� � ������ ������.
   * ������ ���� ���������� �� unique_ptr ��������� �������� ���� ������.
   */
  //  try {
  //    MyClass myclass;
  //  } catch (...) {
  //    cout << "������ ����������." << endl;
  //  }
  //
  // SHARED_PTR
  /*
   * ���� ��� ���������, � ���������� unique_ptr, �������� �����������. ��� ������, ��� �� ����� ���� ����������
   *  � ������ ��� ����� ����� ���������� � ������ � ���� �� ���������. ��� ���������� ������� shared_ptr ������,
   * ���������� � ���, ����� ���������� ����� � ������ �����, ����� �� ���������� ������ �������� shared_ptr �����������
   * �� ���� ������. ��� ������� �������: ��������� ����� ���������� �����, ����� ����� ���������� ��������� �����
   * ������������ shared_ptr, ���� ���� ���� ���� �������. ��� ����������� ����������� �������� ������.
  */
  //� ������� �� unique_ptr shared_ptr �� ���������  ������� � ���� ��������� �� �������,
  //  shared_ptr<int[]> p_arr(new int[5]); //-> ������ ������������
  // �����,��� � unique_ptr, shared_ptr ��������� ������ ���������������� �������� ��������
  HANDLE File2 = ::CreateFile(L"test.txt",
                              GENERIC_WRITE,
                              FILE_SHARE_READ,
                              NULL, CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);
  {
    std::shared_ptr<void> spHandle2(File2, &::CloseHandle);
  }
  /* ��� ��� ���������� ��������� ��� ����� ����������� shared_ptr � ���� ��������� ����� ������� ������ �� ���� ���������,
   * �.�. ��� �������� �������� �� ������ �� ������������ �������! ���, �����, ��������, ��� shared_ptr ��������� � �����
   * ��������� �������� ����� ���� �������� ������� shared_ptr � ������ ��������� ��������. �����, ���, ��� ������� ���� ���
   * ���������������� �������� �������� ����������� � ��� ����������������� ����������.
  */
  //  STD::MAKE_SHARED � std::ALLOCATE_SHARED
  //
  //  ��� ������� ������ ����� ���� � �������� shared_ptr � ������ ����������. allocate_shared ���������� ���� ���,
  //  ��� ��������� ������ ���������������� ���������(�������� ��������� ������) ��� �������� shared_ptr
  //  ��� � make_shared ��������� �� ������������ � ����� ������ new � ��������� ��� ��������� ���� ���� ���.
  //�������� ��������� � ������� new
  std::shared_ptr<A> spFirst(new A(1, 2));
  //�������� ��������� � ������� make_shared
  auto spSecond = std::make_shared<A>(1, 2);
  Q_ASSERT(*spFirst == *spSecond);
  //
  //std::enable_shared_from_this - ������������ �� ����� ������ ��������� �������� shared_ptr �� this
  //������ �� ������������� std::enable_shared_from_this- class Node
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
  // OWNER_BEFORE -�������������� ������ ��������� <.
  // ��������� �����������-��������� ��������� ������������� ������������� ��������� <.
  // ��� ���� ����������� ������� ��������������� ��������� �� ��������� < - ��� shared_ptr(first � second)
  // ������������, ���� ����������� ��������� �������:
  // !first.owner_before(second) &&!second.owner_before(first)
  //
  //#1 - ��� ��������� �������� ���������� < ������� �� �������� ��������������
  {
    auto spInitial = std::make_shared<C>();
    std::shared_ptr<void> spDerived = std::static_pointer_cast<B1>(spInitial);
    Q_ASSERT(spInitial != spDerived);
    Q_ASSERT(spInitial < spDerived || spInitial > spDerived);
    Q_ASSERT(!spInitial.owner_before(spDerived) && !spDerived.owner_before(spInitial));
  }
  //#2 ��� shared_ptr � ������� ������ ������ � �������� < �� ����� ���� ��������, � �� ����� ��� owner_before ������������ ���������.
  auto spInitial = std::make_shared<Z>();
  std::shared_ptr<B1> spInner(spInitial, spInitial->m_B);
  Q_ASSERT(!spInitial.owner_before(spInner) &&
           !spInner.owner_before(spInitial));
  //
  // std::WEAK_PTR
  //�������������� weak_ptr �� ������������� shared_ptr � ��� ������������ ����� �������� ��������������
  //������������ ���������� �� ��������������� � ��� shared_ptr ��� ���������� ��� �������� ������.
  //��������� �������� ����������� ������ ��� ������������� ���������� ��� ����� ��������-�������.
  //��� ���� ��� ��������� �������� �� ������� ������������ shared_ptr, � ��� ��������� ������� �� �������� - weak_ptr
  //weak_ptr �� ��������� �������� � �������� ��������, ������ ����� ������������ ����� lock().
  //
  //����������� shared_ptr
  auto spShared = std::make_shared<int>();
//���� �������� ?
  Q_ASSERT(spShared.unique());
  //�������� weak_ptr �� shared_ptr
  std::weak_ptr<int> Weak(spShared);
  //�������� �� ��������������� shared_ptr ������ ��� �� ������� ������ ������� � ������ �� ���������� �����.
  Q_ASSERT(Weak.expired() == false);
  //���������� ������
  Q_ASSERT(Weak.use_count() == 1);
  //����� �������� ��� ������ shared_ptr ���������������, ������ ��� �����, ��� ���� ���������� ������� ������
  auto spClone = Weak.lock();
  Q_ASSERT(Weak.use_count() == 2);
  //������� ������, ������� ������� ���������
  spShared.reset();
  //�� ���� �������� ?
  Q_ASSERT(!spShared.unique());
  Q_ASSERT(Weak.use_count() == 1);
  spClone.reset();
  Q_ASSERT(Weak.expired());
  //
  //dYNAMIC_POINTER_CAST , STATIC_POINTER_CAST � CONST_POINTER_CAST
  //��������� ����������� ������� ���������� dynamic_cast, static_cast � const_cast ����������� � ������ ����������.
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // ������ � ��������������  shared_ptr:
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  cout << endl << "������ � ��������������  shared_ptr:" << endl;
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
    cout << "������� ������ " << (*it)->get_square() << endl;
  }

  cout << endl << endl;

  for (const auto &it : arr) {
    //���������� ����
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

    cout << "������� ������ " << it->get_square() << endl;
  }

  //���������� ������
  for (auto &it : arr)
    it.reset();

  return 0;
}
