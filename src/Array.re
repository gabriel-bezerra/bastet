open Interface;

module Semigroup: SEMIGROUP_ANY with type t('a) = array('a) = {
  type t('a) = array('a);
  let append = Js.Array.concat
};

module Monoid: MONOID_ANY with type t('a) = array('a) = {
  include Semigroup;
  let empty = [||]
};

module Functor: FUNCTOR with type t('a) = array('a) = {
  type t('a) = array('a);
  let map = (f) => ArrayLabels.map(~f)
};

module Apply: APPLY with type t('a) = array('a) = {
  include Functor;
  let apply = (fn_array, a) =>
    ArrayLabels.fold_left(
      ~f=(acc, f) => ArrayLabels.append(acc, map(f, a)),
      ~init=[||],
      fn_array
    )
};

module Applicative: APPLICATIVE with type t('a) = array('a) = {
  include Apply;
  let pure = (a) => [|a|];
};

module Monad: MONAD with type t('a) = array('a) = {
  include Applicative;
  let flat_map = (x, f) =>
    ArrayLabels.fold_left(
      ~f=(acc, a) => ArrayLabels.append(acc, f(a)),
      ~init=[||],
      x
    )
};

module Foldable: FOLDABLE with type t('a) = array('a) = {
  type t('a) = array('a);
  let fold_left = (f, init) => ArrayLabels.fold_left(~f, ~init);
  let fold_right = (f, init) => ArrayLabels.fold_right(~f, ~init);

  module Fold_Map = (M: MONOID) => {
    module I = Infix.Monoid(M);
    let fold_map = (f, x) => I.(fold_left((acc, x) => acc <:> f(x), M.empty, x));
  };
  module Fold_Map_Any = (M: MONOID_ANY) => {
    module I = Infix.Monoid_Any(M);
    let fold_map = (f, x) => I.(fold_left((acc, x) => acc <:> f(x), M.empty, x));
  };
};