
const STARTING_MISSILES: i32 = 8;  // module scope
const READY_AMOUNT: i32 = 2;

fn main() {

    // this works too:
    /*
    const STARTING_MISSILES: i32 = 8;  // main scope
    const READY_AMOUNT: i32 = 2;
    */

    // let missiles = 8;  // immutable

    // let mut missiles = STARTING_MISSILES;  // mutable
    // let ready = READY_AMOUNT;

    // let mut missiles: i32 = STARTING_MISSILES;  // this type annotation, resembles Python :)
    // let ready: i32 = READY_AMOUNT;

    let (mut missiles, ready): (i32, i32) = (STARTING_MISSILES, READY_AMOUNT);  // not working

    println!("Firing {} of my {} missiles...", ready, missiles);

    missiles = missiles - ready;

    println!("{} missiles left", missiles);


    // println!("{} missiles left", missiles - ready);  // Got a "warning: variable does not need to be mutable" (missiles!!!)

    // let test: i8 = 100;  // warning: unused variable: `test`

    // STARTING_MISSILES = 10;  // error[E0070]: invalid left-hand side of assignment, cannot assign to this expression

}

// expected output:
// Firing 2 of my 8 missiles...
// 6 missiles left