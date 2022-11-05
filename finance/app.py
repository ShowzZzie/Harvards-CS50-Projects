import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
uri = os.getenv("postgres://mwdhvymugboyda:120ff74974b9aee92d9a2a01f11788731333bbfb99866302de53672d36791e32@ec2-176-34-215-248.eu-west-1.compute.amazonaws.com:5432/dcfdqur6kgc6hd")
if uri.startswith("postgres://"):
    uri = uri.replace("postgres://", "postgresql://")
db = SQL(uri)

# postgres://mwdhvymugboyda:120ff74974b9aee92d9a2a01f11788731333bbfb99866302de53672d36791e32@ec2-176-34-215-248.eu-west-1.compute.amazonaws.com:5432/dcfdqur6kgc6hd

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


# API_KEY
# export API_KEY=pk_0579e2f782704b45a9ded9e3e935969b

# THE LOCATION FOR THE HEROKU APP IS:
# /workspaces/attempt2
# TO REACH IT, THE COMMAND IS:
# cd /workspaces/attempt2
# THE LINK TO HEROKU APP IS:
# fake-stock-market.herokuapp.com

@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=['GET', 'POST'])
@login_required
def index():
    if request.method == "GET":
        transactions = db.execute("SELECT symbol, shares, SUM(shares) AS shares FROM transactions WHERE user_id = ? GROUP BY symbol", session["user_id"])
        cashdb = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        if not cashdb:
            return redirect("/login")
        cash = cashdb[0]["cash"]

        prices = {}
        for stock in transactions:
            test1 = lookup(stock["symbol"])
            price = test1["price"]
            elem = stock["symbol"]
            for stock in transactions:
                prices[elem] = price

        total = {}
        for stock in transactions:
            elem = stock["symbol"]
            value = prices[elem] * stock["shares"]
            for stock in transactions:
                total[elem] = value

        names = {}
        for stock in transactions:
            elem = stock["symbol"]
            name1 = lookup(elem)
            name = name1["name"]
            for stock in transactions:
                names[elem] = name

        totalstocks = sum(total.values())

        allassets = float(cash) + totalstocks

        for elem in prices:
            prices[elem] = usd(prices[elem])

        for elem in total:
            total[elem] = usd(total[elem])

        return render_template("index.html", transactions=transactions, cash=usd(cash), prices=prices, total=total, names=names, allassets=usd(allassets), stock=usd(totalstocks))

    else:
        if request.form['button'] == "buy":
            symbol = request.form["stock"]
            shares = request.form.get("shares")

            if not shares:
                flash("Quantity field cannot be empty")
                return redirect('/')

            if shares.isdigit() == False:
                return apology("You can't purchase partial shares")
            if shares.isalpha() == True:
                return apology("Letters are invalid characters for quantity")

            int_shares = int(float(shares))
            if int_shares <= 0:
                flash("Quantity cannot be equal to 0")
                return redirect('/')

            stock = lookup(symbol)
            if not stock:
                return apology("Invalid Stock Symbol")
            data = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
            if not data:
                return apology("Cash Query Failed")

            cash = data[0]["cash"]

            stock_price = round(stock["price"], 2)
            total = stock_price * int_shares

            if (int_shares * stock_price) > cash:
                return apology("You don't have enough cash")

            buy = db.execute("INSERT INTO transactions (user_id, symbol, shares, price, total) VALUES (?, ?, ?, ?, ?)", session["user_id"], symbol, shares, stock_price, total)
            if not buy:
                return apology("Buy Query Error")

            type = "BOUGHT"
            history = db.execute("INSERT INTO history (user_id, type, symbol, shares, price) VALUES (?, ?, ?, ?, ?)", session["user_id"], type, symbol, shares, stock_price)
            if not history:
                return apology("History Query Error")

            spend_cash = db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", total, session["user_id"])
            if not spend_cash:
                return apology("Spending Query Error")

            return redirect("/")

        elif request.form['button'] == "sell":
            symbol = request.form["stock"]
            amount = request.form.get("shares")
            transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])

            if not amount:
                flash("Quantity field cannot be empty")
                return redirect('/')
            if int(amount) == 0:
                flash("Quantity cannot be equal to 0")
                return redirect('/')

            stocks = {}
            for elem in transactions:
                name = elem["symbol"]
                share = elem["shares"]
                for elem in transactions:
                    stocks[name] = share
            # above: dict stocks where key is symbol and value is amount of shares

            if not stocks[symbol]:
                return apology("Invalid symbol")
            if int(amount) > stocks[symbol]:
                return apology("You can't sell more shares than you own!")

            shares = -int(amount)

            stock_price_t = lookup(symbol)
            stock_price = round(stock_price_t["price"], 2)

            # append history table, then remove quantity from transactions
            type = "SOLD"
            history = db.execute("INSERT INTO history (user_id, type, symbol, shares, price) VALUES (?, ?, ?, ?, ?)", session["user_id"], type, symbol, shares, stock_price)
            if not history:
                return apology("History Query Error")

            # remove from transactions
            newshares = stocks[symbol] - int(amount)
            db.execute("UPDATE transactions SET shares = ? WHERE user_id = ? AND symbol = ?", newshares, session["user_id"], symbol)
            if newshares == 0:
                db.execute("DELETE FROM transactions WHERE symbol = ?", symbol)

            # add cash
            cashdb = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
            cash = cashdb[0]["cash"]
            income = int(amount) * stock_price
            new_cash = cash + income

            db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, session["user_id"])

            return redirect("/")




@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "GET":
        return render_template("buy.html")
    else:
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")
        if not symbol:
            return apology("Missing Symbol")
        if not shares:
            return apology("Missing Shares")
        if shares.isdigit() == False:
            return apology("You can't purchase partial shares")
        if shares.isalpha() == True:
            return apology("Letters are invalid characters for quantity")

        int_shares = int(float(shares))
        if int_shares <= 0:
            return apology("Invalid amount of shares")

        stock = lookup(symbol)
        if not stock:
            return apology("Invalid Stock Symbol")
        data = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        if not data:
            return apology("Cash Query Failed")

        cash = data[0]["cash"]

        stock_price = round(stock["price"], 2)
        total = stock_price * int_shares

        if (int_shares * stock_price) > cash:
            return apology("You don't have enough cash")

        buy = db.execute("INSERT INTO transactions (user_id, symbol, shares, price, total) VALUES (?, ?, ?, ?, ?)", session["user_id"], symbol, shares, stock_price, total)
        if not buy:
            return apology("Buy Query Error")

        type = "BOUGHT"
        history = db.execute("INSERT INTO history (user_id, type, symbol, shares, price) VALUES (?, ?, ?, ?, ?)", session["user_id"], type, symbol, shares, stock_price)
        if not history:
            return apology("History Query Error")

        spend_cash = db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", total, session["user_id"])
        if not spend_cash:
            return apology("Spending Query Error")

        return redirect("/")



@app.route("/history")
@login_required
def history():
    historydb = db.execute("SELECT id, type, symbol, shares, price, time FROM history WHERE user_id = ? ORDER BY time DESC", session["user_id"])

    values = {}
    for stock in historydb:
        key = stock['id']
        elem = stock["price"]
        for stock in historydb:
            values[key] = elem

    for elem in values:
        values[elem] = usd(values[elem])

    return render_template("history.html", history=historydb, values=values)
    # first thought - Join tables, order by time



@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():
    cashdb = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = cashdb[0]["cash"]
    if request.method == "GET":
        return render_template("addcash.html", cash=usd(cash))
    else:
        increase = request.form.get("increase")
        if not increase:
            flash("Deposit value cannot be empty")
            return redirect("/addcash")
        if int(increase) <= 0:
            flash("Value must be bigger than 0")
            return redirect("/addcash")
        new = int(increase) + int(cash)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new, session["user_id"])

        inc = int(increase)
        n = "-"
        type = "ADDED CASH"
        history = db.execute("INSERT INTO history (user_id, type, symbol, shares, price) VALUES (?, ?, ?, ?, ?)", session["user_id"], type, n, n, inc)
        if not history:
            return apology("History Query Error")

        flash('Money added successfully')
        return redirect("/addcash")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        session["username"] = request.form.get("username")

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/account")
@login_required
def account():
    name = session["username"]

    return render_template("account.html", name=name)



@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    if request.method == "POST":
        current = request.form.get("current")
        new = request.form.get("new")
        newconfirm = request.form.get("newconfirm")

        passworddb = db.execute("SELECT hash FROM users WHERE id = ?", session["user_id"])

        if not current:
            flash("Current Password field cannot be empty")
            return redirect("/change")
        if not new:
            flash("You must set the new password")
            return redirect("/change")
        if not newconfirm:
            flash("You must repeat the new password")
            return redirect("/change")
        if current == new:
            flash("Your new password cannot match your current password")
            return redirect("/change")

        if not check_password_hash(passworddb[0]["hash"], request.form.get("current")):
            flash("Your current password is incorrect")
            return redirect("/change")


        if len(new) <= 7:
            flash("Your new password must be at least 8 characters long")
            return redirect("/change")

        specials = ["!", "@", '#', '$', '%', '^', '&', '*', '(', ')', '_', '-', '`', '~', "|", '[', ']', '{', '}', ';', ':', ',', '<', '>', '.', '/', "?", '\\']

        check_upper = False
        check_digit = False
        check_specials = False

        for char in new:
            if char.isupper():
                check_upper = True
            elif char.isdigit():
                check_digit = True
            elif char in specials:
                check_specials = True

        if not (check_upper == True and check_digit == True and check_specials == True):
            flash("Your new password must contain at least one capital letter, one digit and one special symbol")
            return redirect('/change')

        if (new != newconfirm):
            flash("New passwords don't match")
            return redirect("/change")

        newhash = generate_password_hash(new)
        if not newhash:
            flash("Hash generation error")
            return redirect("/change")

        db.execute("UPDATE users SET hash = ? WHERE id = ?", newhash, session["user_id"])

        flash("Password changed successfully")
        return redirect("/account")


    else:
        return render_template("change.html")







@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "GET":
        return render_template("quote.html")
    else:
        quote = request.form.get("symbol")
        if not quote:
            flash("Search field cannot be empty")
            return redirect('/quote')

        search = lookup(quote)
        if not search:
            return apology("Query Error")
        search["price"] = usd(search["price"])

        return render_template("quoted.html", search=search)


@app.route("/register", methods=["GET", "POST"])
def register():
    username = request.form.get("username")
    password = request.form.get("password")
    confirmation = request.form.get("confirmation")

    if request.method == "GET":
        return render_template("register.html")
    else:
        if not username:
            flash("The username field cannot be empty!")
            return redirect("/register")
        elif not password:
            flash("The password field cannot be empty!")
            return redirect("/register")
        elif not confirmation:
            flash("You must repeat your password")
            return redirect("/register")
        elif password != confirmation:
            flash("Passwords don't match")
            return redirect("/register")

        if len(password) <= 7:
            flash("Your password must be at least 8 characters long")
            return redirect("/register")

        specials = ["!", "@", '#', '$', '%', '^', '&', '*', '(', ')', '_', '-', '`', '~', "|", '[', ']', '{', '}', ';', ':', ',', '<', '>', '.', '/', "?", '\\']

        check_upper = False
        check_digit = False
        check_specials = False

        for char in password:
            if char.isupper():
                check_upper = True
            elif char.isdigit():
                check_digit = True
            elif char in specials:
                check_specials = True

        if not (check_upper == True and check_digit == True and check_specials == True):
            flash("Your password must contain at least one capital letter, one digit and one special symbol")
            return redirect('/register')

        if password == confirmation:
            hash_p = generate_password_hash(password)
            check = db.execute("SELECT * FROM users WHERE username = ?", username)
            if len(check) != 0:
                flash("This username already exists")
                return redirect("/register")
            else:
                new_user = db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash_p)
                if not new_user:
                    return apology("Query Error")

            login = db.execute("SELECT * FROM users WHERE username = ?", username)
            session["user_id"] = login[0]["id"]
            session["username"] = username

            return redirect("/")

        else:
            return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "GET":
        symbolsform = db.execute("SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0", session["user_id"])
        return render_template("sell.html", symbols=[row["symbol"] for row in symbolsform])
    else:
        symbol = request.form.get("symbol")
        amount = request.form.get("shares")
        transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])

        if not symbol:
            flash("Symbol field cannot be empty")
            return redirect('/sell')
        if not amount:
            flash("Amount field cannot be empty")
            return redirect('/sell')

        stocks = {}
        for elem in transactions:
            name = elem["symbol"]
            share = elem["shares"]
            for elem in transactions:
                stocks[name] = share
        # above: dict stocks where key is symbol and value is amount of shares

        if not stocks[symbol]:
            return apology("Invalid symbol")
        if int(amount) > stocks[symbol]:
            return apology("You can't sell more shares than you own!")

        shares = -int(amount)

        stock_price_t = lookup(symbol)
        stock_price = round(stock_price_t["price"], 2)

        # append history table, then remove quantity from transactions
        type = "SOLD"
        history = db.execute("INSERT INTO history (user_id, type, symbol, shares, price) VALUES (?, ?, ?, ?, ?)", session["user_id"], type, symbol, shares, stock_price)
        if not history:
            return apology("History Query Error")

        # remove from transactions
        newshares = stocks[symbol] - int(amount)
        db.execute("UPDATE transactions SET shares = ? WHERE user_id = ? AND symbol = ?", newshares, session["user_id"], symbol)
        if newshares == 0:
            db.execute("DELETE FROM transactions WHERE symbol = ?", symbol)

        # add cash
        cashdb = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = cashdb[0]["cash"]
        income = int(amount) * stock_price
        new_cash = cash + income

        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, session["user_id"])

        return redirect("/")