/**
 * @file argument.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of Argument data type.
 */
#ifndef _LIBGDL_CORE_DATATYPES_ARGUMENT_HPP
#define _LIBGDL_CORE_DATATYPES_ARGUMENT_HPP

#include <vector>
#include <string>
#include <map>
#include <boost/unordered_map.hpp>

#include <libgdl/core/symbol_table/symbol_table.hpp>

#include <libgdl/gdlparser/parser/token_value.hpp>

namespace libgdl
{
/**
 * Represents argument of fact or clause.
 * Can be relation, function or variable depending on type.
 * Stored as command and arguments.
 *
 * @see KIFDriver
 */
struct Argument
{
  typedef std::vector<std::string> StringVec;
  typedef gdlparser::parser::TokenValue TokenValue;

  //! enum type
  enum Type { Relation, Function, Var };

  //! empty constructor
  Argument() : t(Relation) {}
  //! constucts argument from given token
  Argument(const TokenValue& tok);
  //! copy constructor
  Argument(const Argument& arg) noexcept;
  //! construct argument from string
  Argument(const std::string& str);
  //! move constructor
  Argument(Argument&& arg) noexcept
    : t(arg.t), val(std::move(arg.val)), args(std::move(arg.args)) {}

  //! Destructor
  ~Argument();

  //! swap function
  friend inline void swap(Argument& arg1, Argument& arg2)
  {
    using std::swap;

    swap(arg1.t, arg2.t);
    swap(arg1.val, arg2.val);
    swap(arg1.args, arg2.args);
  }

  //! copy-assignment operator
  Argument& operator=(const Argument& arg);
  //! move assignment constructor
  Argument& operator=(Argument&& arg) { swap(*this, arg); return *this; }

  //! Destroys arguments to this argument before deletion
  //! called by destructor
  //! v_set is the set of variables already deleted
  //! updates it accordingly
  void Destroy(std::set<Argument*>& v_set);

  //! comparison operator
  bool operator==(const Argument& arg) const;
  bool operator!=(const Argument& arg) const { return !(*this == arg); }

  //! special comparison operator
  //! checks value and arguments(recursively check)
  //! for 'or' if given argument matches any argument to 'or' true is returned
  bool OrEquate(const Argument& arg);

  //! return true if given argument is there in arguments
  bool HasAsArgument(const Argument& arg) const;

  //! returns if current argument is ground
  bool IsGround() const;

  //! returns if current argument is a variable
  bool IsVariable() const { if(t == Var) return true; else return false; }

  bool HasVariables() const;

  //! equivalent to comparison operator but 'or' conditions is removed in this
  bool IsEqualTo(const Argument& arg) const;

  //! adds argument to this command
  void AddArgument(const TokenValue& tok) { args.push_back(new Argument(tok)); }

  //! compute hash value
  size_t Hash(const boost::unordered_map<std::string, size_t>& id_map);

  std::string DecodeToString(const SymbolTable& symbol_table) const;

  //! type of this argument
  Type t;
  //! command value
  std::string val;
  size_t value;
  //! vector of arguments
  std::vector<Argument*> args;

  //! used by copy constructors
  static Argument* ConstructArgument(const Argument& arg,
                                     std::map<std::string,
                                     Argument*>& v_map);
  static Argument* ConstructArgument(const TokenValue& tok,
                                     std::map<std::string,
                                     Argument*>& v_map);
  static Argument* ConstructArgument(const std::string& str,
                                     std::map<std::string,
                                     Argument*>& v_map);

  //! separates a string input into command and arguments
  static bool SeparateCommand (const std::string & input,
                               std::string & cmd,
                               std::vector <std::string> & args);

}; // struct Argument

}; //namespace libgdl

inline std::ostream& operator<<(std::ostream& o, const libgdl::Argument& arg)
{
  if(arg.args.size() == 0)
  {
    o << arg.val;
    return o;
  }
  else o << "( " + arg.val;

  for(size_t i = 0;i < arg.args.size();i++) o << " " << *(arg.args[i]);
  o << " " << ")";
  return o;
}

inline std::ostream& operator<<(std::ostream& o, const libgdl::Argument::Type& t)
{
  typedef libgdl::Argument Argument;

  if(t == Argument::Relation) o << "Relation";
  else if(t == Argument::Function) o << "Function";
  else o << "Variable";
  return o;
}


#endif // _LIBGDL_CORE_DATATYPES_ARGUMENT_HPP
