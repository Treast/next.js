let component = ReasonReact.statelessComponent("Index");

let make = (~onServer, _children) => {
  ...component,
  render: _self =>
    <div>
      <Header />
      <p>
        {ReasonReact.string("HOME PAGE is here!")}
        <br />
        {ReasonReact.string("onServer: " ++ string_of_bool(onServer))}
      </p>
      <Counter />
    </div>,
};

let default = ReasonReact.wrapReasonForJs(~component, jsProps => make(~onServer=jsProps##onServer, [||]));

/* The way to do getInitialProps: https://github.com/zeit/next.js/issues/4202#issuecomment-439175214 */
let getInitialProps = context =>
  Js.Promise.make((~resolve, ~reject as _) => {
    let onServer =
      switch (Js.Nullable.toOption(context##req)) {
      | None => false
      | Some(_) => true
      };
    resolve(. {"onServer": onServer});
  });

/* In 7.0.2 canary, you must remove the Js.Promise.make wrapper */
/*
 let getInitialProps = context => {
   let onServer =
     switch (Js.Nullable.toOption(context##req)) {
     | None => false
     | Some(_) => true
     };
   {"onServer": onServer};
 };
 */

let inject = [%bs.raw {| (cls, fn) => cls.getInitialProps = fn |}];

inject(default, getInitialProps);
