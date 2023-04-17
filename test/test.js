// all the logic checking and calling the service
import * as UserServices from "../services/auth.services";
import * as UserValidate from "../schemas/auth.schema";
import { compare, hash } from "bcrypt";
import { signToken } from "../utils/signToken";

async function login(req, res) {
    try {
        console.table(req.body);
        const loginData = UserValidate.parseLogin(req.body);
        if (loginData.success === false) {
            throw new Error(loginData.error.toString());
        }

        const existingUser = await UserServices.getUser(loginData.data.username);
        if (!existingUser) {
            throw new Error("User Not Found, Please Sign Up first!");
        }

        const compredPwd = await compare(
            loginData.data.password,
            existingUser.password
        );
        if (compredPwd === false) {
            throw new Error("Please Login with correct credentials!");
        }

        const payload = {
            user: {
                id: existingUser.id,
                username: existingUser.username,
            },
        };

        const token = await signToken(payload);
        res.cookie("Authorization", token);
        res.json({ status: true, data: { token } });
    } catch (error) {
        if (error instanceof Error) {
            console.error({ error });
            res.json({ status: false, message: error.message });
        }
    }
}

async function signup(req, res) {
    try {
        console.table(req.body);
        const signupData = UserValidate.parseLogin(req.body);
        if (signupData.success === false) {
            throw new Error(signupData.error.toString());
        }
        const existingUser = await UserServices.getUser(signupData.data.username);
        if (existingUser) {
            throw new Error("User already Found!");
        }

        const _password = await hash(signupData.data.password, 10);

        const newCreatedUser = await UserServices.createUser({
            password: _password,
            username: signupData.data.username,
        });
        const payload = {
            user: {
                id: newCreatedUser.id,
                username: newCreatedUser.username,
            },
        };

        const token = await signToken(payload);
        res.cookie("Authorization", token);
        res.json({ status: true, data: { token } });
    } catch (error) {
        if (error instanceof Error) {
            console.error(error.name, error.message);
            res.json({ status: false, message: error.message });
        }
    }
}

async function changePassword(req, res) {
    try {
        const username = req.user.username;
        const password = req.body.password;
        if (!password) {
            throw new Error("Password can not be null!!");
        }
        await UserServices.changePassword({ username, password });
        res.json({
            status: true,
            data: { message: "Password changed sucessfully!!" },
        });
    } catch (error) {
        if (error instanceof Error) {
            console.error(error.name, error.message);
            res.json({ status: false, message: error.message });
        }
    }
}

async function verifyUser(req, res) {
    try {
        const username = req.user.username;
        const user = await UserServices.getUser(username);

        const userData = { ...user, password: "" };
        res.json({
            status: true,
            data: userData,
        });
    } catch (error) {
        if (error instanceof Error) {
            console.error(error.name, error.message);
            res.json({ status: false, message: error.message });
        }
    }
}

export default {
    login,
    signup,
    changePassword,
    verifyUser,
};