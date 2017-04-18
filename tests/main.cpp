#include <yobaperl/perl.hpp>
#include <yobaperl/test.hpp>

using namespace yoba;



int test()
{
   Perl perl;



   Test test(perl);
   // Eval
   {
      perl.eval(" $test = 'pass' ");

      PerlInterpreter * interp = perl.getInterpreter();
      STRLEN len;
      char * ptr = Perl_sv_2pv_flags(interp, Perl_get_sv(interp, "test", 0), &len, 0);
      test.is(std::string(ptr, len), "pass", "Perl::eval<void>");

      test.is(perl.eval<Scalar>("2 * 2").toIV(), 4, "Perl::eval<Scalar>");

      test.is(perl.eval<Array>("1 .. 5").getSize(), 5, "Perl::eval<Array>");
   }

   // Scalar
   {
      Scalar s = perl.newScalar();
      test.pass("Perl::newScalar");

      // Value manipulation
      test.is(s.setValue("str").toString(), "str", "Scalar::setValue(char *), Scalar::toString 1");
      test.is(s.setValue(std::string("str")), "str", "Scalar::setValue(std::string), Scalar::toString() 2");
      test.is(s.makeCopy().toString(), "str", "Scalar::makeCopy()");
      test.is(s.getLength(), 3, "Scalar::getLength 2");
      test.is(s.setValue(IV(123)).toIV(), 123, "Scalar::setValue(IV), Scalar::toIV");
      test.is(s.setValue(UV(-1)).toUV(), UV(-1), "Scalar::setValue(UV), Scalar::toUV");
      test.is(s.setValue(NV(1. / 2.)).toNV(), 0.5, "Scalar::setValue(NV), Scalar::toNV");
      test.ok(s.setValue(true).toBool(), "Scalar::setValue(bool) 1, Scalar::toBool 1");
      test.ok(!s.setValue(false).toBool(), "Scalar::setValue(bool) 2, Scalar::toBool 2");

      // Operators
      test.ok(s, "Scalar::operator bool");
      test.ok(s == s, "Scalar::operator==(Scalar)");
      test.is((s = "str").toString(), "str", "Scalar::operator=(char*)");
      test.is((s = IV(123)).toIV(), 123, "Scalar::operator=(IV)");
      test.ok((s = true).toBool(), "Scalar::operator=(bool)");

      // Scalar creating
      test.is(perl.newScalar("str"), "str", "Perl::newScalar<char *>");
      test.is(perl.newScalar(std::string("str")), "str", "Perl::newScalar<std::string>");
      test.is(perl.newScalar(IV(123)).toIV(), IV(123), "Perl::newScalar<IV>");

      perl.newNamedScalar("varname", "value");
      Scalar named = perl.getScalar("varname");
      test.is(named, "value", "Perl::getScalar");
      test.is(*perl["$varname"], "value", "Perl::operator[]: $");

      // Reference count
      {
         Scalar s = perl.newScalar("val");

         test.is(s.getRefcount(), 1, "Scalar reference count: constructor");

         SV * tmp = nullptr;
         {
            Scalar c = s;
            test.is(c.getRefcount(), 2, "Scalar reference count: copy constructor");
            tmp = c.getSV();
         }
         if(tmp)
            test.is(SvREFCNT(tmp), 1, "Scalar reference count: destructor");

         Scalar b = s.makeCopy();
         test.is(b, "val", "Scalar::makeCopy");
         test.is(b.getRefcount(), 1, "Scalar::makeCopy reference count");

         Scalar m = std::move(s);
         test.is(m, "val", "Scalar move constructor");
         test.is(m.getRefcount(), 1, "Scalar reference count: move constructor");

         perl.eval("undef $var; $var = 'value'");
         Scalar named = perl.getScalar("var");
         test.is(named.getRefcount(), 2, "Perl::getScalar reference count");
      }

      // References
      {
         Scalar scalar = perl.newScalar("value");
         {
            Scalar ref = scalar.makeRef();
            test.ok(ref.isRef(), "Scalar::isRef");
            test.ok(ref.isScalarRef(), "Scalar::isScalarRef");
            test.is(scalar.getRefcount(), 2, "Scalar::makeRef reference count 1");
            test.is(ref.getRefcount(), 1, "Scalar::makeRef reference count 2");
            test.is(scalar, ref.unrefScalar(), "Scalar::unrefScalar");
         }
         test.is(scalar.getRefcount(), 1, "Scalar::makeRef reference count 3");
      }
   }

   // Array
   {
      Array a = perl.newArray();
      test.pass("Perl::newArray");

      test.is(a.push(perl.newScalar("a")), "(a)", "Array::push 1");
      test.is(a.push(perl.newScalar("b")), "(a, b)", "Array::push 2");
      test.is(a.unshift(perl.newScalar("z")), "(z, a, b)", "Array::unshift");
      test.is(a.makeCopy(), "(z, a, b)", "Array::makeCopy");
      test.is(a.getSize(), 3, "Array::getSize");

      test.is(a.toVector().size(), 3, "Array::toVector 1");
      test.is(a[0], a.toVector()[0], "Array::toVector 2");
      test.is(a[2], a.toVector()[2], "Array::toVector 3");
      test.is(a.toList().size(), 3, "Array::toList 1");
      test.is(a.getFirst(), a.toList().front(), "Array::toList 2");
      test.is(a.getLast(), a.toList().back(), "Array::toList 3");

      test.is(a.pop(), "b", "Array::pop 1");
      test.is(a.getSize(), 2, "Array::pop 2");
      test.is(a.replace(0, perl.newScalar("x")).get(0), "x", "Array::replace");
      test.is(a.shift(), "x", "Array::shift 1");
      test.is(a.getSize(), 1, "Array::shift 2");

      // Operators
      test.ok(a, "Array::operator bool");
      test.ok(a == a, "Array::operator==(Array)");

      test.is(a.clear(), "()", "Array::clear");

      // Array creating
      test.is(perl.newArray({ "a", "b", "c" }), "(a, b, c)", "Perl::newArray<char *>");
      test.is(perl.newArray({ 1, 2, 3 }), "(1, 2, 3)", "Perl::newArray<int>");
      test.is(perl.newArray({1, 2}).push(perl.newArray({3, 4})), "(1, 2, 3, 4)", "Array::push(Array)");
      test.is(perl.newArray({1, 2}).push(std::vector<Scalar> { perl.newScalar(3), perl.newScalar(4) }), "(1, 2, 3, 4)", "Array::push(std::vector<Scalar>)");
      test.is(perl.newArray({1, 2}).push(std::list<Scalar> { perl.newScalar(3), perl.newScalar(4) }), "(1, 2, 3, 4)", "Array::push(std::list<Scalar>)");
      test.is(perl.newArray({1, 2}).unshift(perl.newArray({3, 4})), "(3, 4, 1, 2)", "Array::unshift(Array)");
      test.is(perl.newArray({1, 2}).unshift(std::vector<Scalar> { perl.newScalar(3), perl.newScalar(4) }), "(3, 4, 1, 2)", "Array::unshift(std::vector<Scalar>)");
      test.is(perl.newArray({1, 2}).unshift(std::list<Scalar> { perl.newScalar(3), perl.newScalar(4) }), "(3, 4, 1, 2)", "Array::unshift(std::list<Scalar>)");
      test.is(perl.newArray(1, 3), "(1, 2, 3)", "Perl::newArray range");

      perl.newNamedArray("varname", { 1, 2, 3 });
      Array named = perl.getArray("varname");
      test.is(named, "(1, 2, 3)", "Perl::getArray");
      test.ok(perl.getNamedSV("varname"), "Perl::getNamedSV");
      test.is(*perl["@varname"], "(1, 2, 3)", "Perl::operator[]: @");

      //Reference count
      {
         Array a = perl.newArray({ 1, 2 });

         Scalar s = perl.newScalar(3);
         a.push(s);
         test.is(s.getRefcount(), 2, "Array element reference count");

         test.is(a.getRefcount(), 1, "Array reference count: constructor");

         AV * tmp = nullptr;
         {
            Array c = a;
            test.is(c.getRefcount(), 2, "Array reference count: copy constructor");
            tmp = c.getAV();
         }
         if(tmp)
            test.is(SvREFCNT(tmp), 1, "Array reference count: destructor");

         Array b = a.makeCopy();
         test.is(b, "(1, 2, 3)", "Array::makeCopy");
         test.is(b.getRefcount(), 1, "Array::makeCopy reference count");

         Array m = std::move(a);
         test.is(m, "(1, 2, 3)", "Array move constructor");
         test.is(m.getRefcount(), 1, "Array reference count: move constructor");

         perl.eval("@var = (1, 2)");
         test.ok(perl.getNamedAV("var"), "Perl::getNamedAV");
         Array named = perl.getArray("var");
         test.is(named, "(1, 2)", "Perl::getArray");
         test.is(named.getRefcount(), 2, "Perl::getArray reference count");
      }

      // Iteration
      {
         Array a = perl.newArray({ "a", "b", "c" });
         Array::Iterator it = a.begin();

         test.is((*it), "a", "Array::Iterator::operator* 1");
         ++it;
         test.is((*it), "b", "Array::Iterator::operator++, Array::Iterator::operator* 2");
         it += 2;
         test.ok(it == a.end(), "Array::Iterator::operator+=");
      }

      // References
      {
         Array array = perl.newArray( {"a", "b"} );
         {
            Scalar ref = array.makeRef();
            test.ok(ref.isRef(), "Scalar::isRef");
            test.ok(ref.isArrayRef(), "Scalar::isArrayRef");
            test.is(array.getRefcount(), 2, "Array::makeRef reference count 1");
            test.is(ref.getRefcount(), 1, "Array::makeRef reference count 2");
            test.is(array, ref.unrefArray(), "Scalar::unrefArray");
         }
         test.is(array.getRefcount(), 1, "Array::makeRef reference count 3");
      }
   }

   // Hash
   {
      Hash h = perl.newHash<IV>({ { "a", 1}, { "b", 2} });
      test.is(h.get("a"), 1, "Perl::newHash<IV> 1");
      test.is(h.get("b"), 2, "Perl::newHash<IV> 2");
      h.insert("c", perl.newScalar(3));
      test.is(h.get("c"), 3, "Hash::insert");
      test.is(h.getSize(), 3, "Hash::getSize");
      test.is(h.toMap().size(), 3, "Hash::toMap 1");
      test.is(h.get("a"), h.toMap().at("a"), "Hash::toMap 2");
      test.ok(h.isExists("b"), "Hash::isExists 1");
      h.remove("b");
      test.is(h.getSize(), 2, "Hash::remove");
      test.ok(!h.isExists("b"), "Hash::isExists 2");

      // Operators
      test.is(h["c"], 3, "Hash::operator[]");

      h.clear();
      test.is(h.getSize(), 0, "Hash::clear");

      // Hash creating
      test.is(perl.newHash<IV>({ { "a", 1} }), "(a => 1)", "Perl::newHash<IV>");
      test.is(perl.newHash().insert(std::make_pair("a", perl.newScalar(1))), "(a => 1)", "Hash::insert(std::pair)");
      test.is(perl.newHash().insert(std::unordered_map<std::string, Scalar> { { "a", perl.newScalar(1) } }), "(a => 1)", "Hash::insert(std::unordered_map)");

      perl.newNamedHash<IV>("varname", { { "a", 1 } });
      Hash named = perl.getHash("varname");
      test.is(named, "(a => 1)", "Perl::getHash");
      test.ok(perl.getNamedHV("varname"), "Perl::getNamedHV");
      test.is(*perl["%varname"], "(a => 1)", "Perl::operator[]: %");

      // Reference count
      {
         Hash h = perl.newHash<IV>({ { "a", 1 } });

         Scalar s = perl.newScalar(2);
         h.insert("b", s);
         test.is(s.getRefcount(), 2, "Hash element reference count");

         test.is(h.getRefcount(), 1, "Hash reference count: constructor");

         HV * tmp = nullptr;
         {
            Hash c = h;
            test.is(c.getRefcount(), 2, "Hash reference count: copy constructor");
            tmp = c.getHV();
         }
         if(tmp)
            test.is(SvREFCNT(tmp), 1, "Hash reference count: destructor");

         Hash b = h.makeCopy();
         test.is(b.get("a"), h.get("a"), "Hash::makeCopy 1");
         test.is(b.get("b"), h.get("b"), "Hash::makeCopy 2");
         test.is(b.getRefcount(), 1, "Hash::makeCopy reference count");

         Hash m = std::move(h);
         test.is(m.getRefcount(), 1, "Hash reference count: move constructor");

         perl.eval("undef %var; %var = (a => 1)");
         Hash named = perl.getHash("var");
         test.is(named.getRefcount(), 2, "Perl::getHash reference count");
      }

      // Iteration
      {
         Hash h = perl.newHash<IV>({ { "a", 1 }, { "b", 2 }, { "c", 3 } });
         Hash::Iterator it = h.begin();

         test.ok(it, "Hash::begin");
         test.ok(!(*it).getKey().empty(), "Hash::Iterator::operator* 1");
         ++it;
         test.ok((*it).getValue(), "Hash::Iterator::operator++, Hash::Iterator::operator* 2");
         it += 2;
         test.ok(it == h.end(), "Hash::Iterator::operator+=, Hash::end");
      }

      // References
      {
         Hash hash = perl.newHash<IV>({ {"a", 1} });
         {
            Scalar ref = hash.makeRef();
            test.ok(ref.isRef(), "Scalar::isRef");
            test.ok(ref.isHashRef(), "Scalar::isHashRef");
            test.is(hash.getRefcount(), 2, "Hash::makeRef reference count 1");
            test.is(ref.getRefcount(), 1, "Hash::makeRef reference count 2");
            test.is(hash, ref.unrefHash(), "Scalar::unrefHash");
         }
         test.is(hash.getRefcount(), 1, "Hash::makeRef reference count 3");
      }
   }

   // Objects
   {
      perl.eval("$test = bless {}, 'Yoba'");
      test.ok(perl.getScalar("test").isObject(), "Scalar::isObject");
      test.ok(perl.getScalar("test").isObjectOf("Yoba"), "Scalar::isObjectOf");
   }

   // Subroutines
   {
      perl.eval(" $test = 'old'; sub test { $test = 'new' } ");
      perl.call<void>("test");
      test.is(perl.getScalar("test"), "new", "Perl::call<void>");

      test.ok(perl.getNamedCV("test"), "Perl::getNamedCV");
      test.ok(!perl["&test"]->toString().empty(), "Perl::operator[]: &");

      perl.eval(" $test = 'old'; sub test { $test = $_[0] } ");
      perl.call<void, Scalar>("test", perl.newScalar("new"));
      test.is(perl.getScalar("test"), "new", "Perl::call<void, Scalar>");

      perl.eval(" $test = 'old'; sub test { $test = join '', @_ } ");
      perl.call<void, Array>("test", perl.newArray({ "ne", "w" }));
      test.is(perl.getScalar("test"), "new", "Perl::call<void, Array>");

      perl.eval(" sub test { 'pass' } ");
      test.is(perl.call<Scalar>("test"), "pass", "Perl::call<Scalar>");

      perl.eval(" sub test { $_[0] } ");
      test.is(perl.call<Scalar, Scalar>("test", perl.newScalar("pass")), "pass", "Perl::call<Scalar, Array>");

      perl.eval(" sub test { join '', @_ } ");
      test.is(perl.call<Scalar, Array>("test", perl.newArray({ "pa", "ss" })), "pass", "Perl::call<Scalar, Array>");

      perl.eval(" sub test { 'pa', 'ss' } ");
      test.is(perl.call<Array>("test"), "(pa, ss)", "Perl::call<Array>");

      perl.eval(" sub test { split //, $_[0] } ");
      test.is(perl.call<Array, Scalar>("test", perl.newScalar("ab")), "(a, b)", "Perl::call<Array, Scalar>");

      perl.eval(" sub test { reverse @_ } ");
      test.is(perl.call<Array, Array>("test", perl.newArray({ "b", "a" })), "(a, b)", "Perl::call<Array, Array>");

      {
         perl.eval(" sub test { $tmp = $_[0] } ");
         Scalar param = perl.newScalar("value");
         perl.call<void, Scalar>("test", param);
         test.is(param, "value", "Perl::call<void, Scalar> reference count 1");
         test.is(param.getRefcount(), 1, "Perl::call<void, Scalar> reference count 2");
      }

      {
         perl.eval(" sub test { $tmp = $_[0] } ");
         Scalar param = perl.newScalar("value");
         Array params = perl.newArray({ param });
         perl.call<void, Array>("test", params);
         test.is(param, "value", "Perl::call<void, Array> reference count 1");
         test.is(param, params.get(0), "Perl::call<void, Array> reference count 2");
         test.is(param.getRefcount(), 2, "Perl::call<void, Array> reference count 3");
         test.is(params.getRefcount(), 1, "Perl::call<void, Array> reference count 4");
      }
   }

   // Objects
   {
      perl.lib("../../tests");
      perl.lib("../tests");
      perl.require("'class.pl'");

      Scalar o = perl.newObject("Yoba");
      test.ok(o.isObjectOf("Yoba"), "Perl::newObject(classname)");
      Scalar o2 = perl.newObject("Yoba", perl.newScalar("param"));
      test.ok(o2.isObjectOf("Yoba"), "Perl::newObject(classname, Scalar param)");
      Scalar o3 = perl.newObject("Yoba", perl.newArray({1, 2, 3}));
      test.ok(o3.isObjectOf("Yoba"), "Perl::newObject(classname, Array params)");

      perl.callMethod<void>("void_void", o);
      perl.callMethod<void, Scalar>("void_scalar", o, perl.newScalar("param"));
      perl.callMethod<void, Array>("void_array", o, perl.newArray({ 1, 2, 3 }));
      test.is(perl.callMethod<Scalar>("scalar_void", o), "result", "Perl:callMethod<Scalar>");
      test.is(perl.callMethod<Scalar, Scalar>("scalar_scalar", o, perl.newScalar("param")), "result", "Perl::callMethod<Scalar, Scalar>");
      test.is(perl.callMethod<Scalar, Array>("scalar_array", o, perl.newArray({ 1, 2, 3 })), "result", "Perl::callMethod<Scalar, Array>");
      test.is(perl.callMethod<Array>("array_void", o), "(1, 2, 3)", "Perl::callMethod<Array>");
      test.is(perl.callMethod<Array, Scalar>("array_scalar", o, perl.newScalar("param")), "(1, 2, 3)", "Perl::callMethod<Array, Scalar>");
      test.is(perl.callMethod<Array, Array>("array_array", o, perl.newArray({ 1, 2, 3 })), "(1, 2, 3)", "Perl::callMethod<Array, Array>");
   }

   perl.use("Socket");
   test.pass("Load a XS module");

   return test.doneTesting();
}

#include <thread>
int main()
{
#ifdef YOBAPERL_MULTIPLICITY
   std::thread t1(&test);
   std::thread t2(&test);

   t1.join();
   t2.join();
#else
   test();
#endif
}
