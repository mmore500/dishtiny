#!/usr/bin/env python3

import git
import os
import random
import sys
import tempfile
import textwrap
import tweepy

from itertools import chain, combinations, islice

def get_hexsha():
    try:
        repo = git.Repo(
            os.path.realpath(__file__),
            search_parent_directories=True,
        )
        return repo.head.object.hexsha[:7]
    except Exception as e:
        print(e)

    try:
        repo = git.Repo(
            f'{os.getenv("REPRO_DIR")}/dishtiny/'
        )
        return repo.head.object.hexsha[:7]
    except Exception as e:
        print(e)

    return '???????'


def make_attribution():

    return textwrap.dedent(f'''
    I tweet data from experiments where computer programs evolve!

    🌐 https://mmore500.com/dishtiny/

    🗃️ https://osf.io/dnh2v/

    👥 @MorenoMatthewA

    🔣 #evolutioninaction #icermsu #scicomm #openscience

    🆔 {os.getenv("SLURM_JOB_ID")}

    📌 {get_hexsha()}
    ''')

def powerset(iterable):
    "powerset([1,2,3]) --> () (1,) (2,) (3,) (1,2) (1,3) (2,3) (1,2,3)"
    s = list(iterable)
    return chain.from_iterable(combinations(s, r) for r in range(len(s)+1))

def make_catchphrase(which):
    catchprases = [
        f'peep my {which}type 🐣',
        f'new {which}type who dis 💬',
        f'{which}type on FLEEK 💅',
        f"they see my {which}type evolvin' 😏",
        f'I am once again asking you to look at my {which}type 🧤',
        f'today we will be learning about my {which}type 👩‍🏫',
        f'what I evolved in boating school was this {which}type 🛥️',
        f'is this... ... a {which}type? 🦋',
        f"I don't always tweet from a supercomputer, but when I do it's my {which}type 🍻",
        f'no one // absolutely no one // my {which}type: 🕸 🐠 📗 🗳 🎷',
        f'{which}tonks 📈',
        f'Houston, we have a {which}type 🚀',
        f'{which}typdt. 😤',
        f'CEO of {which}type 💼',
        f'ok {which}type 🙄',
        f'{which}type check ✨',
        f'{which}type glow up 💫',
        f'yassss {which}type, SLAY 💪',
        f'good morning but only to my {which}type ☕',
        '👏 phe 👏 no 👏 type 👏',
        f'{which}types, amirite 🙄',
        f'umm mam, this is a {which}type 🥡',
        f'boaty mc{which}typeface 😜',
        f'just a {which}typical day in the life 🤷',
        f'maybe the real {which}type was the friends we made along the way 💖',
        f"I'm not like a regular {which}type, I'm a cool {which}type! 😎",
        f'404 {which}type not found 🦄',
        f'{which}typie!!! 🤳',
        f"friends don't let friends skip {which}type day 🏋️",
    ]
    return random.choice(catchprases)


def make_id(series):
    return series % 1000

def make_name(series):
    alphabet = ['🅰', '🅱️', '©️', '↩️', '📧,' '🎏', '⛽️', '♓️', 'ℹ️', '🗾', '🎋', '👢', '♏️', '♑️', '🅾', '️🅿️', '🕠', '®️', '💲', '✝️', '⛎', '♈️', '〰️', '❎', '💹', '⚡️', ]

    id = make_id(series)
    return next(islice(powerset(alphabet), id+1, None))[0]

def make_tweepy_handle():

    TWITTER_API_KEY = os.getenv('TWITTER_API_KEY')
    TWITTER_API_SECRET_KEY = os.getenv('TWITTER_API_SECRET_KEY')
    TWITTER_ACCESS_TOKEN = os.getenv('TWITTER_ACCESS_TOKEN')
    TWITTER_ACCESS_TOKEN_SECRET = os.getenv('TWITTER_ACCESS_TOKEN_SECRET')

    print('TWITTER_API_KEY', '*'*len(TWITTER_API_KEY))
    print('TWITTER_API_SECRET_KEY', '*'*len(TWITTER_API_SECRET_KEY))
    print('TWITTER_ACCESS_TOKEN', '*'*len(TWITTER_ACCESS_TOKEN))
    print('TWITTER_ACCESS_TOKEN_SECRET', '*'*len(TWITTER_ACCESS_TOKEN_SECRET))

    auth = tweepy.OAuthHandler( TWITTER_API_KEY, TWITTER_API_SECRET_KEY )
    auth.set_access_token( TWITTER_ACCESS_TOKEN, TWITTER_ACCESS_TOKEN_SECRET )

    # Creation of the actual interface, using authentication
    return tweepy.API(auth)
